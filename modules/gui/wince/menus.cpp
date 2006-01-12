/*****************************************************************************
 * menus.cpp : WinCE gui plugin for VLC
 *****************************************************************************
 * Copyright (C) 2000-2004 the VideoLAN team
 * $Id$
 *
 * Authors: Marodon Cedric <cedric_marodon@yahoo.fr>
 *          Gildas Bazin <gbazin@videolan.org>
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

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>                                            /* strerror() */
#include <stdio.h>
#include <vlc/vlc.h>
#include <vlc/intf.h>

#include "wince.h"

/*****************************************************************************
 * Event Table.
 *****************************************************************************/

/* IDs for the controls and the menu commands */
enum
{
    /* menu items */
    MenuDummy_Event = 1000,
    OpenFileSimple_Event = 1100,
    OpenFile_Event,
    OpenDisc_Event,
    OpenNet_Event,
    FirstAutoGenerated_Event = 1999,
    SettingsMenu_Events = 2000,
    AudioMenu_Events = 3000,
    VideoMenu_Events = 4000,
    NavigMenu_Events = 5000,
    PopupMenu_Events = 6000
};

HMENU OpenStreamMenu( intf_thread_t *p_intf )
{
    HMENU hmenu = CreatePopupMenu();
    AppendMenu( hmenu, MF_STRING, ID_FILE_QUICKOPEN,
                _T("Quick &Open File...") );
    AppendMenu( hmenu, MF_STRING, ID_FILE_OPENFILE,
                _T("Open &File...") );
    AppendMenu( hmenu, MF_STRING, ID_FILE_OPENNET,
                _T("Open &Network Stream...") );
    return hmenu;
}

HMENU MiscMenu( intf_thread_t *p_intf )
{
    HMENU hmenu = CreatePopupMenu();
    AppendMenu( hmenu, MF_STRING, ID_VIEW_STREAMINFO, _T("Media &Info...") );
    AppendMenu( hmenu, MF_STRING, ID_VIEW_MESSAGES, _T("&Messages...") );
    AppendMenu( hmenu, MF_STRING, ID_PREFERENCES, _T("&Preferences...") );
    return hmenu;
}

void PopupMenu( intf_thread_t *p_intf, HWND p_parent, POINT point )
{
#define MAX_POPUP_ITEMS 45

    vlc_object_t *p_object, *p_input;
    char *ppsz_varnames[MAX_POPUP_ITEMS];
    int pi_objects[MAX_POPUP_ITEMS];
    int i = 0, i_last_separator = 0;

    /* Initializations */
    memset( pi_objects, 0, MAX_POPUP_ITEMS * sizeof(int) );

    ppsz_varnames[i] = "VLC media player";
    pi_objects[i++] = 0;
    ppsz_varnames[i++] = NULL; /* Separator */
    i_last_separator = i;

    /* Input menu */
    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_INPUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "bookmark";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "title";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "chapter";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "program";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "navigation";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "dvd_menus";
        pi_objects[i++] = p_object->i_object_id;

        ppsz_varnames[i] = "video-es";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "audio-es";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "spu-es";
        pi_objects[i++] = p_object->i_object_id;
    }
    p_input = p_object;
    if( !p_input ) goto interfacemenu;

    /* Video menu */
    if( i != i_last_separator ) ppsz_varnames[i++] = NULL; /* Separator */
    i_last_separator = i;

    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_VOUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        vlc_object_t *p_dec_obj;

        ppsz_varnames[i] = "fullscreen";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "zoom";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "deinterlace";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "aspect-ratio";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "crop";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "video-on-top";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "directx-wallpaper";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "video-snapshot";
        pi_objects[i++] = p_object->i_object_id;

        p_dec_obj = (vlc_object_t *)vlc_object_find( p_object,
                                                     VLC_OBJECT_DECODER,
                                                     FIND_PARENT );
        if( p_dec_obj != NULL )
        {
            ppsz_varnames[i] = "ffmpeg-pp-q";
            pi_objects[i++] = p_dec_obj->i_object_id;
            vlc_object_release( p_dec_obj );
        }

        vlc_object_release( p_object );
    }

    /* Audio menu */
    if( i != i_last_separator ) ppsz_varnames[i++] = NULL; /* Separator */
    i_last_separator  = i;

    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_AOUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "audio-device";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "audio-channels";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "visual";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "equalizer";
        pi_objects[i++] = p_object->i_object_id;
        vlc_object_release( p_object );
    }

 interfacemenu:
    /* Interface menu */
    if( i != i_last_separator ) ppsz_varnames[i++] = NULL; /* Separator */
    i_last_separator = i;

    /* vlc_object_find is needed because of the dialogs provider case */
    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_INTF,
                                                FIND_PARENT );
    if( p_object != NULL )
    {
        /* Nothing for now */
        vlc_object_release( p_object );
    }

    /* Build menu */
    vector<MenuItemExt*> popup_menu;
    HMENU hmenu = CreatePopupMenu();
    RefreshMenu( p_intf, &popup_menu, hmenu, i,
                 ppsz_varnames, pi_objects, PopupMenu_Events );
    MenuItemExt::ClearList( &popup_menu );


    /* Add static entries */
    if( p_input != NULL )
    {
        vlc_value_t val;
        AppendMenu( hmenu, MF_SEPARATOR, 0, _T("") );
        AppendMenu( hmenu, MF_STRING, StopStream_Event, _T("Stop") );
        AppendMenu( hmenu, MF_STRING, PrevStream_Event, _T("Previous") );
        AppendMenu( hmenu, MF_STRING, NextStream_Event, _T("Next") );

        var_Get( p_input, "state", &val );
        if( val.i_int == PAUSE_S )
            AppendMenu( hmenu, MF_STRING, PlayStream_Event, _T("Play") );
        else
            AppendMenu( hmenu, MF_STRING, PlayStream_Event, _T("Pause") );

        vlc_object_release( p_input );
    }
    else
    {
        playlist_t * p_playlist =
            (playlist_t *)vlc_object_find( p_intf, VLC_OBJECT_PLAYLIST,
                                           FIND_ANYWHERE );
        if( p_playlist && p_playlist->i_size )
        {
            AppendMenu( hmenu, MF_SEPARATOR, 0, _T("") );
            AppendMenu( hmenu, MF_STRING, PlayStream_Event, _T("Play") );
        }
        if( p_playlist ) vlc_object_release( p_playlist );
    }

    AppendMenu( hmenu, MF_POPUP|MF_STRING, (UINT)MiscMenu( p_intf ),
                _T("Miscellaneous") );
    AppendMenu( hmenu, MF_POPUP|MF_STRING, (UINT)OpenStreamMenu( p_intf ),
                _T("Open") );

    TrackPopupMenu( hmenu, 0, point.x, point.y, 0, p_parent, 0 );
    PostMessage( p_parent, WM_NULL, 0, 0 );
    DestroyMenu( hmenu );
}

void RefreshAudioMenu( intf_thread_t *p_intf, HMENU hMenu )
{
#define MAX_AUDIO_ITEMS 10

    vlc_object_t *p_object;
    char *ppsz_varnames[MAX_AUDIO_ITEMS];
    int pi_objects[MAX_AUDIO_ITEMS];
    int i;

    /* Delete old menu */
    int count = wce_GetMenuItemCount( hMenu );
    for( i = 0; i <= count; i++ ) RemoveMenu( hMenu, 0, MF_BYPOSITION );

    if( p_intf->p_sys->p_audio_menu )
        MenuItemExt::ClearList( p_intf->p_sys->p_audio_menu );
    else p_intf->p_sys->p_audio_menu = new vector<MenuItemExt*>;


    /* Initializations */
    memset( pi_objects, 0, MAX_AUDIO_ITEMS * sizeof(int) );
    i = 0;

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_INPUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "audio-es";
        pi_objects[i++] = p_object->i_object_id;
        vlc_object_release( p_object );
    }

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_AOUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "audio-device";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "audio-channels";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "visual";
        pi_objects[i++] = p_object->i_object_id;
        vlc_object_release( p_object );
    }

    /* Build menu */
    RefreshMenu( p_intf, p_intf->p_sys->p_audio_menu,
                 hMenu, i, ppsz_varnames, pi_objects, AudioMenu_Events );
}

void RefreshVideoMenu( intf_thread_t *p_intf, HMENU hMenu )
{
#define MAX_VIDEO_ITEMS 15

    vlc_object_t *p_object;
    char *ppsz_varnames[MAX_VIDEO_ITEMS];
    int pi_objects[MAX_VIDEO_ITEMS];
    int i;

    /* Delete old menu */
    int count = wce_GetMenuItemCount( hMenu );
    for( i = 0; i <= count; i++ ) RemoveMenu( hMenu, 0, MF_BYPOSITION );

    if( p_intf->p_sys->p_video_menu )
        MenuItemExt::ClearList( p_intf->p_sys->p_video_menu );
    else p_intf->p_sys->p_video_menu = new vector<MenuItemExt*>;

    /* Initializations */
    memset( pi_objects, 0, MAX_VIDEO_ITEMS * sizeof(int) );
    i = 0;

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_INPUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "video-es";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "spu-es";
        pi_objects[i++] = p_object->i_object_id;
        vlc_object_release( p_object );
    }

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_VOUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        vlc_object_t *p_dec_obj;

        ppsz_varnames[i] = "fullscreen";
        pi_objects[i++] = p_object->i_object_id;
#ifdef WINCE
        ppsz_varnames[i] = "transform";
        pi_objects[i++] = p_object->i_object_id;
#endif
        ppsz_varnames[i] = "zoom";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "deinterlace";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "aspect-ratio";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "crop";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "directx-on-top";
        pi_objects[i++] = p_object->i_object_id;

        p_dec_obj = (vlc_object_t *)
            vlc_object_find( p_object, VLC_OBJECT_DECODER, FIND_PARENT );
        if( p_dec_obj != NULL )
        {
            ppsz_varnames[i] = "ffmpeg-pp-q";
            pi_objects[i++] = p_dec_obj->i_object_id;
            vlc_object_release( p_dec_obj );
        }

        vlc_object_release( p_object );
    }

    /* Build menu */
    RefreshMenu( p_intf, p_intf->p_sys->p_video_menu, hMenu, i,
                 ppsz_varnames, pi_objects, VideoMenu_Events );
}

void RefreshNavigMenu( intf_thread_t *p_intf, HMENU hMenu )
{
#define MAX_NAVIG_ITEMS 10

    vlc_object_t *p_object;
    char *ppsz_varnames[MAX_NAVIG_ITEMS];
    int pi_objects[MAX_NAVIG_ITEMS];
    int i;

    /* Delete old menu */
    int count = wce_GetMenuItemCount( hMenu );
    for( i = 0; i <= count; i++ ) RemoveMenu( hMenu, 0, MF_BYPOSITION );
        
    if( p_intf->p_sys->p_navig_menu )
        MenuItemExt::ClearList( p_intf->p_sys->p_navig_menu );
    else p_intf->p_sys->p_navig_menu = new vector<MenuItemExt*>;

    /* Initializations */
    memset( pi_objects, 0, MAX_NAVIG_ITEMS * sizeof(int) );
    i = 0;

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_INPUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "title";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "chapter";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "program";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "navigation";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "dvd_menus";
        pi_objects[i++] = p_object->i_object_id;

        ppsz_varnames[i] = "prev-title";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "next-title";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "prev-chapter";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "next-chapter";
        pi_objects[i++] = p_object->i_object_id;

        vlc_object_release( p_object );
    }

    /* Build menu */
    RefreshMenu( p_intf, p_intf->p_sys->p_navig_menu, hMenu, i,
                 ppsz_varnames, pi_objects, NavigMenu_Events );
}

void RefreshSettingsMenu( intf_thread_t *p_intf, HMENU hMenu )
{
#define MAX_SETTINGS_ITEMS 10

    vlc_object_t *p_object;
    char *ppsz_varnames[MAX_SETTINGS_ITEMS];
    int pi_objects[MAX_SETTINGS_ITEMS];
    int i;

    /* Delete old menu */
    int count = wce_GetMenuItemCount( hMenu );
    for( i = 0; i <= count; i++ ) RemoveMenu( hMenu, 0, MF_BYPOSITION );

    if( p_intf->p_sys->p_settings_menu )
        MenuItemExt::ClearList( p_intf->p_sys->p_settings_menu );
    else p_intf->p_sys->p_settings_menu = new vector<MenuItemExt*>;

    /* Initializations */
    memset( pi_objects, 0, MAX_SETTINGS_ITEMS * sizeof(int) );
    i = 0;

    AppendMenu( hMenu, MF_STRING, ID_PREFERENCES, _T("&Preferences...") );

    p_object = (vlc_object_t *)
        vlc_object_find( p_intf, VLC_OBJECT_INTF, FIND_PARENT );
    if( p_object != NULL )
    {
        ppsz_varnames[i] = "intf-switch";
        pi_objects[i++] = p_object->i_object_id;
        ppsz_varnames[i] = "intf-add";
        pi_objects[i++] = p_object->i_object_id;
        vlc_object_release( p_object );
    }

    /* Build menu */
    RefreshMenu( p_intf, p_intf->p_sys->p_settings_menu, hMenu, i,
                 ppsz_varnames, pi_objects, SettingsMenu_Events );
}

/*****************************************************************************
 * Refresh the menu.
 *****************************************************************************/
void RefreshMenu( intf_thread_t *p_intf, vector<MenuItemExt*> *p_menu_list,
                  HMENU hMenu , int i_count, char **ppsz_varnames, 
                  int *pi_objects, int i_start_id )
{
    vlc_object_t *p_object;
    vlc_bool_t b_section_empty = VLC_FALSE;
    int i;

    /* Initializations */
    int i_item_id = i_start_id;

    for( i = 0; i < i_count; i++ )
    {
        if( !ppsz_varnames[i] )
        {
            if( b_section_empty )
            {
                AppendMenu( hMenu, MF_GRAYED | MF_STRING,
                            MenuDummy_Event + i, _T("Empty") );
            }

            AppendMenu( hMenu, MF_SEPARATOR, 0, _T("") );
            b_section_empty = VLC_TRUE;
            continue;
        }

        if( !pi_objects[i] )
        {
            AppendMenu( hMenu, MF_GRAYED | MF_STRING,
                        MenuDummy_Event, _FROMMB(ppsz_varnames[i]) );

            b_section_empty = VLC_FALSE;
            continue;
        }

        p_object = (vlc_object_t *)vlc_object_get( p_intf, pi_objects[i] );
        if( p_object == NULL ) continue;

        b_section_empty = VLC_FALSE;
        CreateMenuItem( p_intf, p_menu_list, hMenu, ppsz_varnames[i],
                        p_object, &i_item_id );
        vlc_object_release( p_object );
    }

    /* Special case for empty menus */
    if( wce_GetMenuItemCount(hMenu) == 0 || b_section_empty )
    {
        AppendMenu( hMenu, MF_GRAYED | MF_STRING,
                    MenuDummy_Event + i, _T("Empty") );
    }
}

/*****************************************************************************
 * Private methods.
 *****************************************************************************/
void CreateMenuItem( intf_thread_t *p_intf, vector<MenuItemExt*> *p_menu_list,
                     HMENU hMenu, char *psz_var, vlc_object_t *p_object,
                     int *pi_item_id )
{
    MenuItemExt *pMenuItemExt;
    HMENU hMenuItem;
    vlc_value_t val, text;
    int i_type;

    /* Check the type of the object variable */
    i_type = var_Type( p_object, psz_var );

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
    case VLC_VAR_BOOL:
    case VLC_VAR_VARIABLE:
    case VLC_VAR_STRING:
    case VLC_VAR_INTEGER:
    case VLC_VAR_FLOAT:
        break;
    default:
        /* Variable doesn't exist or isn't handled */
        return;
    }

    /* Make sure we want to display the variable */
    if( i_type & VLC_VAR_HASCHOICE )
    {
        var_Change( p_object, psz_var, VLC_VAR_CHOICESCOUNT, &val, NULL );
        if( val.i_int == 0 ) return;
        if( (i_type & VLC_VAR_TYPE) != VLC_VAR_VARIABLE && val.i_int == 1 )
            return;
    }

    /* Get the descriptive name of the variable */
    var_Change( p_object, psz_var, VLC_VAR_GETTEXT, &text, NULL );

    var_Get( p_object, psz_var, &val );

    if( i_type & VLC_VAR_HASCHOICE )
    {
        hMenuItem = CreateChoicesMenu( p_intf, p_menu_list, psz_var,
                                       p_object, pi_item_id );
        AppendMenu( hMenu, MF_STRING | MF_POPUP, (UINT)hMenuItem,
                    _FROMMB(text.psz_string ? text.psz_string : psz_var) );
        if( (i_type & VLC_VAR_TYPE) == VLC_VAR_STRING ) free( val.psz_string );
        if( text.psz_string ) free( text.psz_string );
        return;
    }

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
        AppendMenu( hMenu, MF_STRING , ++(*pi_item_id),
                    _FROMMB(text.psz_string ? text.psz_string : psz_var) );
        pMenuItemExt = new MenuItemExt( p_intf, *pi_item_id, psz_var,
                                        p_object->i_object_id, val, i_type );
        p_menu_list->push_back( pMenuItemExt );
        break;

    case VLC_VAR_BOOL:
        val.b_bool = !val.b_bool;
        AppendMenu( hMenu, MF_STRING | MF_CHECKED, ++(*pi_item_id),
                    _FROMMB(text.psz_string ? text.psz_string : psz_var) );
        pMenuItemExt = new MenuItemExt( p_intf, *pi_item_id, psz_var,
                                        p_object->i_object_id, val, i_type );
        p_menu_list->push_back( pMenuItemExt );
        CheckMenuItem( hMenu, *pi_item_id ,
                       ( val.b_bool ? MF_UNCHECKED : MF_CHECKED ) |
                       MF_BYCOMMAND ); 
        break;

    default:
        if( text.psz_string ) free( text.psz_string );
        return;
    }

    if( (i_type & VLC_VAR_TYPE) == VLC_VAR_STRING ) free( val.psz_string );
    if( text.psz_string ) free( text.psz_string );
}

HMENU CreateChoicesMenu( intf_thread_t *p_intf,
                         vector<MenuItemExt*> *p_menu_list, char *psz_var, 
                         vlc_object_t *p_object, int *pi_item_id )
{
    MenuItemExt *pMenuItemExt;
    vlc_value_t val, val_list, text_list;
    int i_type, i;
    HMENU hSubMenu = CreatePopupMenu();

    /* Check the type of the object variable */
    i_type = var_Type( p_object, psz_var );

    /* Make sure we want to display the variable */
    if( i_type & VLC_VAR_HASCHOICE )
    {
        var_Change( p_object, psz_var, VLC_VAR_CHOICESCOUNT, &val, NULL );
        if( val.i_int == 0 ) return NULL;
        if( (i_type & VLC_VAR_TYPE) != VLC_VAR_VARIABLE && val.i_int == 1 )
            return NULL;
    }
    else
    {
        return NULL;
    }

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
    case VLC_VAR_BOOL:
    case VLC_VAR_VARIABLE:
    case VLC_VAR_STRING:
    case VLC_VAR_INTEGER:
        break;
    default:
        /* Variable doesn't exist or isn't handled */
        return NULL;
    }

    if( var_Get( p_object, psz_var, &val ) < 0 ) return NULL;

    if( var_Change( p_object, psz_var, VLC_VAR_GETLIST,
                    &val_list, &text_list ) < 0 )
    {
        if( (i_type & VLC_VAR_TYPE) == VLC_VAR_STRING ) free( val.psz_string );
        return NULL;
    }

    for( i = 0; i < val_list.p_list->i_count; i++ )
    {
        vlc_value_t another_val;
        HMENU hMenuItem;
        char *psz_tmp;

        switch( i_type & VLC_VAR_TYPE )
        {
        case VLC_VAR_VARIABLE:
            hMenuItem = CreateChoicesMenu( p_intf, p_menu_list,
              val_list.p_list->p_values[i].psz_string, p_object, pi_item_id );
            AppendMenu( hSubMenu, MF_STRING | MF_POPUP, (UINT)hMenuItem,
                        _FROMMB(text_list.p_list->p_values[i].psz_string ?
                          text_list.p_list->p_values[i].psz_string :
                          val_list.p_list->p_values[i].psz_string) );
            break;

        case VLC_VAR_STRING:
            another_val.psz_string =
                strdup(val_list.p_list->p_values[i].psz_string);
            AppendMenu( hSubMenu, MF_STRING, ++(*pi_item_id),
                        _FROMMB(text_list.p_list->p_values[i].psz_string ?
                          text_list.p_list->p_values[i].psz_string :
                          val_list.p_list->p_values[i].psz_string) );
            pMenuItemExt = new MenuItemExt( p_intf, *pi_item_id, psz_var,
                          p_object->i_object_id, another_val, i_type );
            p_menu_list->push_back( pMenuItemExt );

            if( !(i_type & VLC_VAR_ISCOMMAND) && val.psz_string &&
                !strcmp( val.psz_string,
                         val_list.p_list->p_values[i].psz_string ) )
              CheckMenuItem( hSubMenu, *pi_item_id, MF_CHECKED | MF_BYCOMMAND);
            break;

        case VLC_VAR_INTEGER:
            asprintf( &psz_tmp, "%d", val_list.p_list->p_values[i].i_int );
            AppendMenu( hSubMenu, MF_STRING, ++(*pi_item_id),
                        _FROMMB(text_list.p_list->p_values[i].psz_string ?
                          text_list.p_list->p_values[i].psz_string : psz_tmp));
            pMenuItemExt = new MenuItemExt( p_intf, *pi_item_id, psz_var,
                p_object->i_object_id, val_list.p_list->p_values[i], i_type );
            p_menu_list->push_back( pMenuItemExt );

            if( val_list.p_list->p_values[i].i_int == val.i_int )
              CheckMenuItem( hSubMenu, *pi_item_id, MF_CHECKED | MF_BYCOMMAND);
            break;

        default:
            break;
        }
    }

    /* Clean up everything */
    if( (i_type & VLC_VAR_TYPE) == VLC_VAR_STRING ) free( val.psz_string );
    var_Change( p_object, psz_var, VLC_VAR_FREELIST, &val_list, &text_list );

    return hSubMenu;
}

int wce_GetMenuItemCount(HMENU hMenu)
{
    const int MAX_NUM_ITEMS = 256;
    int iPos, iCount;

    MENUITEMINFO mii;
    memset( (char *)&mii, 0, sizeof(MENUITEMINFO) );
    mii.cbSize = sizeof(MENUITEMINFO);

    iCount = 0;
    for( iPos = 0; iPos < MAX_NUM_ITEMS; iPos++ )
    {
        if( !GetMenuItemInfo(hMenu, (UINT)iPos, TRUE, &mii) ) break;
        iCount++;
    }

    return iCount;
}

void OnMenuEvent( intf_thread_t *p_intf, int id )
{
    MenuItemExt *p_menuitemext = NULL;
    vector<MenuItemExt*>::iterator iter;

    if( p_intf->p_sys->p_settings_menu )
    for( iter = p_intf->p_sys->p_settings_menu->begin();
         iter != p_intf->p_sys->p_settings_menu->end(); iter++ )
        if( (*iter)->id == id )
        {
            p_menuitemext = *iter;
            break;
        }

    if( p_intf->p_sys->p_audio_menu && !p_menuitemext )
    for( iter = p_intf->p_sys->p_audio_menu->begin();
         iter != p_intf->p_sys->p_audio_menu->end(); iter++ ) 
        if( (*iter)->id == id )
        {
            p_menuitemext = *iter;
            break;
        }

    if( p_intf->p_sys->p_video_menu && !p_menuitemext )
    for( iter = p_intf->p_sys->p_video_menu->begin();
         iter != p_intf->p_sys->p_video_menu->end(); iter++ )
        if( (*iter)->id == id )
        {
            p_menuitemext = *iter;
            break;
        }

    if( p_intf->p_sys->p_navig_menu && !p_menuitemext )
    for( iter = p_intf->p_sys->p_navig_menu->begin();
         iter != p_intf->p_sys->p_navig_menu->end(); iter++ )
        if( (*iter)->id == id )
        {
            p_menuitemext = *iter;
            break;
        }

    if( p_menuitemext )
    {
        vlc_object_t *p_object = (vlc_object_t *)
            vlc_object_get( p_intf, p_menuitemext->i_object_id );
        if( p_object == NULL ) return;

        var_Set( p_object, p_menuitemext->psz_var, p_menuitemext->val );
        int i_type = var_Type( p_object, p_menuitemext->psz_var );
        switch( i_type & VLC_VAR_TYPE )
        {
        case VLC_VAR_VOID:
        case VLC_VAR_BOOL:
        case VLC_VAR_VARIABLE:
        case VLC_VAR_STRING:
        case VLC_VAR_INTEGER:
            break;
        default:
            /* Variable doesn't exist or isn't handled */
            return;
        }

        vlc_object_release( p_object );
    }
}

/*****************************************************************************
 * A small helper class which encapsulate wxMenuitem with some other useful
 * things.
 *****************************************************************************/
MenuItemExt::MenuItemExt( intf_thread_t *p_intf, int _id, char *_psz_var,
                          int _i_object_id, vlc_value_t _val, int _i_val_type )
{
    /* Initializations */
    id = _id;
    p_intf = p_intf;
    psz_var = strdup( _psz_var );
    i_val_type = _i_val_type;
    i_object_id = _i_object_id;
    val = _val;
};

MenuItemExt::~MenuItemExt()
{
    if( psz_var ) free( psz_var );
    if( ((i_val_type & VLC_VAR_TYPE) == VLC_VAR_STRING)
        && val.psz_string ) free( val.psz_string );
};

void MenuItemExt::ClearList( vector<MenuItemExt*> *p_menu_list )
{
    vector<MenuItemExt*>::iterator iter;

    if( !p_menu_list ) return;
    for( iter = p_menu_list->begin(); iter != p_menu_list->end(); iter++ )
    {
        delete *iter;
    }
    p_menu_list->clear();
}
