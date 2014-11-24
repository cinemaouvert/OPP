/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "videowidget.h"

#include <QtGlobal>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QToolBar>
#include <QPainter>

#if defined(Q_OS_MAC)
    #import <Cocoa/Cocoa.h>
#elif defined(Q_WS_X11)
    #include <X11/Xlib.h>
    #include <qx11info_x11.h>
#endif

#if defined(Q_OS_MAC)
/************ INTERFACE ***************/
@interface CustomView : NSView {
    NSPoint location;

    NSColor *itemColor;

    NSColor *backgroundColor;

    id delegate;
    NSColor * backColor;
    BOOL stretchesVideo;

}

- (id)initWithFrame:(NSRect)rect;

/****** Draw the View Content *******/

- (void)drawRect:(NSRect)rect;

    - (void)addVoutSubview:(NSView *)view;
   - (void)removeVoutSubview:(NSView *)view;

/* Properties */
@property (assign) id delegate;
@property (copy) NSColor *backColor;
@property BOOL fillScreen;

@end // End interface prototype


/************** IMPLEMENTATION ****************/
@implementation CustomView

    - (id)initWithFrame:(NSRect)rect {
        if (self = [super initWithFrame:rect])
        {
            self.delegate = nil;
            self.backColor = [NSColor blackColor];
            self.fillScreen = NO;
            [self setAutoresizesSubviews:YES];
        }

        return self;
    }

    // Release the View
    - (void)dealloc
    {
        // release the color items and set
        // the instance variables to nil

        [itemColor release];

        itemColor=nil;

        [backgroundColor release];

        backgroundColor=nil;

        self.delegate = nil;
        self.backColor = nil;

        // call super
        [super dealloc];
    }

    // Draw the View Content
    - (void)drawRect:(NSRect)rect
    {
        // Fill in background Color
        CGContextRef context = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
        CGContextSetRGBFillColor(context, 0.0, 0.0, 0.0, 1.0);
        CGContextFillRect(context, NSRectToCGRect(rect));
    }

    - (void)addVoutSubview:(NSView *)view
      {
            [view setFrame:[self bounds]];
            [self addSubview:view];
            [view setAutoresizingMask: NSViewHeightSizable |NSViewWidthSizable];
     }
            - (void)removeVoutSubview:(NSView *)view
            {
            [view removeFromSuperview];
            }

    /* Properties */
    @synthesize delegate;
    @synthesize backColor;

@end
/************ END ******************/
#endif

VideoWidget::VideoWidget(QWidget *parent)
    :
    #if defined(Q_OS_MAC)
          QMacCocoaViewContainer(0, parent)
    #else
          QWidget(parent)
    #endif
{
    init();
}

VideoWidget::~VideoWidget()
{
    release();

#if !defined(Q_OS_MAC)
    delete _layout;

    /* Ensure we are not leaking the video output. This would crash. */
    Q_ASSERT(!_video);
#endif
}

void VideoWidget::init()
{
#if defined(Q_OS_MAC)
    CustomView *video = [[CustomView alloc] init];

    //CustomView *content = [[CustomView alloc] init];
    //[video addSubview:content];
    //[content setAutoresizingMask: NSViewHeightSizable|NSViewWidthSizable];

    /***** A ENLEVER, NON UTILISE *********/
    NSInteger osxVersion;
    if (floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_6) {
        //10.6.x or earlier systems
        osxVersion = 106;
        //NSLog(@"Mac OSX Snow Leopard");
    } else if (floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_7) {
        /* On a 10.7 - 10.7.x system */
        osxVersion = 107;
        //NSLog(@"Mac OSX Lion");
    } else if (floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_8) {
        /* On a 10.8 - 10.8.x system */
        osxVersion = 108;
        //NSLog(@"Mac OSX Moutain Lion");
    } else {
        /* 10.9 or later system */
        osxVersion = 109;
        //NSLog(@"Mac OSX: Mavericks or Later");
    }
    /***************************************/

    setCocoaView(video);

    // Used by libvlc to change the aspect ratio (IMPORTANT)

    [video setAutoresizesSubviews: YES];
    [video setAutoresizingMask: NSViewHeightSizable|NSViewWidthSizable];

    [video release];

#else
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = 0;

    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    setPalette(plt);
#endif
}

void VideoWidget::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId VideoWidget::request()
{
#if defined(Q_OS_MAC)
    return WId(cocoaView());
#else
    if (_video)
        return 0;

    _video = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    _video->setPalette(plt);
    _video->setAutoFillBackground(true);
    _video->setMouseTracking(true);
# ifndef Q_WS_X11
    _video->setAttribute( Qt::WA_PaintOnScreen, true );
# endif
    _layout->addWidget(_video);
    sync();

    return _video->winId();
#endif
}

void VideoWidget::release()
{
#if !defined(Q_OS_MAC)
    if (_video) {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }
#endif
    updateGeometry();
}
