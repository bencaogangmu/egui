/**
 * @file
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of EDUGUI.
 *
 * EDUGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/
# include "right_click_menu.h"

/* 右键菜单 */
# define MENU_NUM 3

/* 右键菜单 */
static struct button * menu_button[MENU_NUM];
static si_t menu_show = 0;

extern struct image_view * Desktop_im;
/*桌面应用图标数*/
extern const si_t APP_NUMBER;
extern struct icon** ic_desktop_ptr;

si_t
right_click_menu_NEW_FOLDER
(void * bt,
 void * msg)
{
    switch(message_get_type(msg))
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_LEFT:
            break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_MID:
	    break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_RIGHT:
	    break;

        default:
            button_default_callback(bt, msg);
            break;
    }
    return 0;
}

si_t
right_click_menu_NEW_FILE
(void * bt,
 void * msg)
{
    switch(message_get_type(msg))
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_LEFT:
            break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_MID:
	    break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_RIGHT:
	    break;

        default:
            button_default_callback(bt, msg);
            break;
    }
    return 0;
}

si_t
right_click_menu_FLUSH
(void * bt,
 void * msg)
{
    switch(message_get_type(msg))
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_LEFT:
	    desktop_flush();
            break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_MID:
	    break;

        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK_RIGHT:
	    break;

        default:
            button_default_callback(bt, msg);
            break;
    }
    return 0;
}

si_t right_click_menu_init() {
	si_t i = 0;
	for(i=0;i<MENU_NUM;i++)
		menu_button[i]=NULL;
	
	menu_button[0] = button_init("NEW FOLDER");
	/* 申请失败 */
	if(menu_button[0] == NULL)
	{
		application_exit();
		return -1;
	}
	window_set_color(menu_button[0], NULL, &light_green);
    	button_set_bounds(menu_button[0], 0, -30, 0, 0);
	menu_button[0]->callback = right_click_menu_NEW_FOLDER;    
	/* 添加button */   
	object_attach_child(OBJECT_POINTER(Desktop_im), OBJECT_POINTER(menu_button[0]));
	
	menu_button[1] = button_init("NEW FILE");
	/* 申请失败 */
	if(menu_button[1] == NULL)
	{
		application_exit();
		return -1;
	}
	window_set_color(menu_button[1], NULL, &light_green);
    	button_set_bounds(menu_button[1], 0, -30, 0, 0);
	menu_button[1]->callback = right_click_menu_NEW_FILE;    
	/* 添加button */   
	object_attach_child(OBJECT_POINTER(Desktop_im), OBJECT_POINTER(menu_button[1]));
	
	menu_button[2] = button_init("FLUSH");
	/* 申请失败 */
	if(menu_button[2] == NULL)
	{
		application_exit();
		return -1;
	}
	window_set_color(menu_button[2], NULL, &light_green);
    	button_set_bounds(menu_button[2], 0, -30, 0, 0);
	menu_button[2]->callback = right_click_menu_FLUSH;    
	/* 添加button */   
	object_attach_child(OBJECT_POINTER(Desktop_im), OBJECT_POINTER(menu_button[2]));
	
	/* 右键菜单不显示 */	
	menu_show = 0;
}

void right_click_menu_handle(union message* msg) {
	si_t i=0;
	if(menu_show == 0){
		struct point* point_ptr=message_get_cursor_position(msg);
		si_t x=point_ptr->x, y=point_ptr->y;
		for(i=0; i<MENU_NUM; i++){
			button_set_bounds(menu_button[i], x, y-30, 100, 30);
	    		y+=30;
	    	}
		
		/* 所有图标重绘 */
    		
		for(i=0; i<MENU_NUM; i++){
			button_repaint(menu_button[i]);
			button_show(menu_button[i]);
	    	}
	    	
		/* 右键菜单显示 */	
		menu_show = 1;
	}
	else{
		for(i=0;i<MENU_NUM;i++){
			button_set_bounds(menu_button[i],0, -30, 0, 0);
		}
	
		desktop_flush();
		
		for(i=0; i<MENU_NUM; i++){
			button_repaint(menu_button[i]);
			button_show(menu_button[i]);
	    	}
		
		/* 右键菜单不显示 */
		menu_show = 0;		
	}
}

void right_click_menu_cancel() {
		si_t i;
		for(i=0;i<MENU_NUM;i++){
			button_set_bounds(menu_button[i],0, -30, 0, 0);
		}
		
		desktop_flush();
			
		for(i=0; i<MENU_NUM; i++){
			button_repaint(menu_button[i]);
			button_show(menu_button[i]);
	    	}
	    		    	
		/* 右键菜单不显示 */
		menu_show = 0;	
}

void desktop_flush(){
	/* 桌面刷新重绘 */
	image_view_reshow(Desktop_im);

	/* 所有图标重绘 */

	for(si_t i=0;i<APP_NUMBER;i++){
	icon_repaint(ic_desktop_ptr[i]);
	icon_show(ic_desktop_ptr[i]);
	}
}
