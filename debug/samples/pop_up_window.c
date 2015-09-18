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

# include <stdio.h>
# include <string.h>
# include <unistd.h>

# include "geometry.h"
# include "comm.h"
# include "application.h"
# include "client_lib.h"
# include "widget.h"

static struct color light_green = {0xcc, 0xff, 0x99, 0};
static struct color barely_blue = {0xcb, 0xf3, 0xfb, 0};

si_t
button_callback
(void * btn,
 void * msg)
{
    struct window * w;

    switch(message_get_type(msg))
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            /* 申请窗口 */
            w = window_init("son_window");
            /* 申请失败 */
            if(w == NULL)
            {
                application_exit();
                return -1;
            }
			window_set_bounds(w, 100, 100, 300, 100);
			window_set_color(w, NULL, &barely_blue);

            /* 添加子窗口 */
            application_add_window(NULL, w);
            break;

        default:
            button_default_callback(btn, msg);
            break;
    }

    return 0;
}



int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w = NULL;
    struct button * b = NULL;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "pop_up_window");

    /* 申请窗口 */
    w = window_init("pop_up");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 448, 200);
	window_set_color(w, NULL, &light_green);

    /* 申请按钮 */
    b = button_init("click me!");
    /* 申请失败 */
    if(b == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b, 50, 50, 100, 50);
	button_set_color(b, NULL, &barely_blue);
    b->callback = button_callback;
    
	/* 添加button */   
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
