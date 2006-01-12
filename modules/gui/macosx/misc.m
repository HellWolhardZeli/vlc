/*****************************************************************************
 * misc.m: code not specific to vlc
 *****************************************************************************
 * Copyright (C) 2003-2005 the VideoLAN team
 * $Id$
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <Cocoa/Cocoa.h>

#include "intf.h"                                          /* VLCApplication */
#include "misc.h"
#include "playlist.h"

/*****************************************************************************
 * VLCControllerWindow
 *****************************************************************************/

@implementation VLCControllerWindow

- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)styleMask
    backing:(NSBackingStoreType)backingType defer:(BOOL)flag
{
    self = [super initWithContentRect:contentRect styleMask:styleMask //& ~NSTitledWindowMask
    backing:backingType defer:flag];

    [[VLCMain sharedInstance] updateTogglePlaylistState];

    return( self );
}

- (BOOL)performKeyEquivalent:(NSEvent *)o_event
{
    return [[VLCMain sharedInstance] hasDefinedShortcutKey:o_event];
}

@end



/*****************************************************************************
 * VLCControllerView
 *****************************************************************************/

@implementation VLCControllerView

- (void)dealloc
{
    [self unregisterDraggedTypes];
    [super dealloc];
}

- (void)awakeFromNib
{
    [self registerForDraggedTypes:[NSArray arrayWithObjects:NSTIFFPboardType, 
        NSFilenamesPboardType, nil]];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    if ((NSDragOperationGeneric & [sender draggingSourceOperationMask]) 
                == NSDragOperationGeneric)
    {
        return NSDragOperationGeneric;
    }
    else
    {
        return NSDragOperationNone;
    }
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
    return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    NSPasteboard *o_paste = [sender draggingPasteboard];
    NSArray *o_types = [NSArray arrayWithObjects: NSFilenamesPboardType, nil];
    NSString *o_desired_type = [o_paste availableTypeFromArray:o_types];
    NSData *o_carried_data = [o_paste dataForType:o_desired_type];

    if( o_carried_data )
    {
        if ([o_desired_type isEqualToString:NSFilenamesPboardType])
        {
            int i;
            NSArray *o_array = [NSArray array];
            NSArray *o_values = [[o_paste propertyListForType: NSFilenamesPboardType]
                        sortedArrayUsingSelector:@selector(caseInsensitiveCompare:)];

            for( i = 0; i < (int)[o_values count]; i++)
            {
                NSDictionary *o_dic;
                o_dic = [NSDictionary dictionaryWithObject:[o_values objectAtIndex:i] forKey:@"ITEM_URL"];
                o_array = [o_array arrayByAddingObject: o_dic];
            }
            [(VLCPlaylist *)[[VLCMain sharedInstance] getPlaylist] appendArray: o_array atPos: -1 enqueue:NO];
            return YES;
        }
    }
    [self setNeedsDisplay:YES];
    return YES;
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
    [self setNeedsDisplay:YES];
}

@end

/*****************************************************************************
 * VLBrushedMetalImageView
 *****************************************************************************/

@implementation VLBrushedMetalImageView

- (BOOL)mouseDownCanMoveWindow
{
    return YES;
}

- (void)dealloc
{
    [self unregisterDraggedTypes];
    [super dealloc];
}

- (void)awakeFromNib
{
    [self registerForDraggedTypes:[NSArray arrayWithObjects:NSTIFFPboardType, 
        NSFilenamesPboardType, nil]];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    if ((NSDragOperationGeneric & [sender draggingSourceOperationMask]) 
                == NSDragOperationGeneric)
    {
        return NSDragOperationGeneric;
    }
    else
    {
        return NSDragOperationNone;
    }
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
    return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    NSPasteboard *o_paste = [sender draggingPasteboard];
    NSArray *o_types = [NSArray arrayWithObjects: NSFilenamesPboardType, nil];
    NSString *o_desired_type = [o_paste availableTypeFromArray:o_types];
    NSData *o_carried_data = [o_paste dataForType:o_desired_type];

    if( o_carried_data )
    {
        if ([o_desired_type isEqualToString:NSFilenamesPboardType])
        {
            int i;
            NSArray *o_array = [NSArray array];
            NSArray *o_values = [[o_paste propertyListForType: NSFilenamesPboardType]
                        sortedArrayUsingSelector:@selector(caseInsensitiveCompare:)];

            for( i = 0; i < (int)[o_values count]; i++)
            {
                NSDictionary *o_dic;
                o_dic = [NSDictionary dictionaryWithObject:[o_values objectAtIndex:i] forKey:@"ITEM_URL"];
                o_array = [o_array arrayByAddingObject: o_dic];
            }
            [[[VLCMain sharedInstance] getPlaylist] appendArray: o_array atPos: -1 enqueue:NO];
            return YES;
        }
    }
    [self setNeedsDisplay:YES];
    return YES;
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
    [self setNeedsDisplay:YES];
}

@end


/*****************************************************************************
 * MPSlider
 *****************************************************************************/
@implementation MPSlider

void _drawKnobInRect(NSRect knobRect)
{
    // Center knob in given rect
    knobRect.origin.x += (int)((float)(knobRect.size.width - 7)/2.0);
    knobRect.origin.y += (int)((float)(knobRect.size.height - 7)/2.0);
    
    // Draw diamond
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 3, knobRect.origin.y + 6, 1, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 2, knobRect.origin.y + 5, 3, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 1, knobRect.origin.y + 4, 5, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 0, knobRect.origin.y + 3, 7, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 1, knobRect.origin.y + 2, 5, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 2, knobRect.origin.y + 1, 3, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(knobRect.origin.x + 3, knobRect.origin.y + 0, 1, 1), NSCompositeSourceOver);
}

void _drawFrameInRect(NSRect frameRect)
{
    // Draw frame
    NSRectFillUsingOperation(NSMakeRect(frameRect.origin.x, frameRect.origin.y, frameRect.size.width, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(frameRect.origin.x, frameRect.origin.y + frameRect.size.height-1, frameRect.size.width, 1), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(frameRect.origin.x, frameRect.origin.y, 1, frameRect.size.height), NSCompositeSourceOver);
    NSRectFillUsingOperation(NSMakeRect(frameRect.origin.x+frameRect.size.width-1, frameRect.origin.y, 1, frameRect.size.height), NSCompositeSourceOver);
}

- (void)drawRect:(NSRect)rect
{
    // Draw default to make sure the slider behaves correctly
    [[NSGraphicsContext currentContext] saveGraphicsState];
    NSRectClip(NSZeroRect);
    [super drawRect:rect];
    [[NSGraphicsContext currentContext] restoreGraphicsState];
    
    // Full size
    rect = [self bounds];
    int diff = (int)(([[self cell] knobThickness] - 7.0)/2.0) - 1;
    rect.origin.x += diff-1;
    rect.origin.y += diff;
    rect.size.width -= 2*diff-2;
    rect.size.height -= 2*diff;
    
    // Draw dark
    NSRect knobRect = [[self cell] knobRectFlipped:NO];
    [[[NSColor blackColor] colorWithAlphaComponent:0.6] set];
    _drawFrameInRect(rect);
    _drawKnobInRect(knobRect);
    
    // Draw shadow
    [[[NSColor blackColor] colorWithAlphaComponent:0.1] set];
    rect.origin.x++;
    rect.origin.y++;
    knobRect.origin.x++;
    knobRect.origin.y++;
    _drawFrameInRect(rect);
    _drawKnobInRect(knobRect);
}

@end


/*****************************************************************************
 * ITSlider
 *****************************************************************************/

@implementation ITSlider

- (void)awakeFromNib
{
    if ([[self cell] class] != [ITSliderCell class]) {
        // replace cell
        NSSliderCell *oldCell = [self cell];
        NSSliderCell *newCell = [[[ITSliderCell alloc] init] autorelease];
        [newCell setTag:[oldCell tag]];
        [newCell setTarget:[oldCell target]];
        [newCell setAction:[oldCell action]];
        [newCell setControlSize:[oldCell controlSize]];
        [newCell setType:[oldCell type]];
        [newCell setState:[oldCell state]]; 
        [newCell setAllowsTickMarkValuesOnly:[oldCell allowsTickMarkValuesOnly]];
        [newCell setAltIncrementValue:[oldCell altIncrementValue]];
        [newCell setControlTint:[oldCell controlTint]];
        [newCell setKnobThickness:[oldCell knobThickness]];
        [newCell setMaxValue:[oldCell maxValue]];
        [newCell setMinValue:[oldCell minValue]];
        [newCell setDoubleValue:[oldCell doubleValue]];
        [newCell setNumberOfTickMarks:[oldCell numberOfTickMarks]];
        [newCell setEditable:[oldCell isEditable]];
        [newCell setEnabled:[oldCell isEnabled]];
        [newCell setEntryType:[oldCell entryType]];
        [newCell setHighlighted:[oldCell isHighlighted]];
        [newCell setTickMarkPosition:[oldCell tickMarkPosition]];
        [self setCell:newCell];
    }
}

@end

/*****************************************************************************
 * ITSliderCell
 *****************************************************************************/
@implementation ITSliderCell

- (id)init
{
    self = [super init];
    _knobOff = [[NSImage imageNamed:@"volumeslider_normal"] retain];
    _knobOn = [[NSImage imageNamed:@"volumeslider_blue"] retain];
    b_mouse_down = FALSE;
    return self;
}

- (void)dealloc
{
    [_knobOff release];
    [_knobOn release];
    [super dealloc];
}

- (void)drawKnob:(NSRect)knob_rect
{
    NSImage *knob;

    if( b_mouse_down )
        knob = _knobOn;
    else
        knob = _knobOff;

    [[self controlView] lockFocus];
    [knob compositeToPoint:NSMakePoint( knob_rect.origin.x + 1,
        knob_rect.origin.y + knob_rect.size.height -2 )  
        operation:NSCompositeSourceOver];
    [[self controlView] unlockFocus];
}

- (void)stopTracking:(NSPoint)lastPoint at:(NSPoint)stopPoint inView: 
        (NSView *)controlView mouseIsUp:(BOOL)flag
{
    b_mouse_down = NO;
    [self drawKnob];
    [super stopTracking:lastPoint at:stopPoint inView:controlView mouseIsUp:flag];
}

- (BOOL)startTrackingAt:(NSPoint)startPoint inView:(NSView *)controlView
{
    b_mouse_down = YES;
    [self drawKnob];
    return [super startTrackingAt:startPoint inView:controlView];
}

@end

