/**
 * @file icon.c
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
# include <stdlib.h>
# include <string.h>
# include <fcntl.h> /* open() */
# include <unistd.h> /* close() */

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>
# include <graph.h>
# include "compiler.h"
# include "application.h"

# include "icon.h"

/* TODO load default parameters like other widgets */

void *icon_init(si_t id)
{
    struct icon * addr;

    /* 分配存储空间 */
    addr = (struct icon *)malloc(sizeof(struct icon));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), id))) {
        return NULL;
    }
    addr->name = "struct icon";

	addr->img_field.x = 0;
	addr->img_field.y = 0;
	addr->img_field.width = 0;
	addr->img_field.height = 0;

	addr->text_field.x = 0;
	addr->text_field.y = 0;
	addr->text_field.width = 0;
	addr->text_field.height = 0;

	addr->is_text_visiable = 0;
	addr->flag = 0;
	memset(addr->img_path , 0 ,sizeof(char)*255);
	memset(addr->img_normal_path , 0 ,sizeof(char)*255);
	memset(addr->img_select_path , 0 ,sizeof(char)*255);
	memset(addr->text , 0, sizeof(char)*255);
    /* 默认的回调函数 */
    addr->callback = icon_default_callback;

    return addr;
}

/*
 * @breif 本函数功能是通过判断
 *
 */
si_t   icon_update_all_areas ( struct icon * ic )
{
	struct rectangle area;
	/* 总字数 ， 单字宽度，单字高度，每行字数 ,总行数,最后一行的字数 */
	si_t char_total_num ;
	si_t font_height;
	si_t font_width;
	si_t text_line_num;
	si_t maxlen;

    application_widget_absolute_area(WIDGET_POINTER(ic), &area);


	if(icon_get_is_text_visiable(ic) == 1)
	{
		/* 总字数 ， 单字宽度，单字高度 */
		char_total_num = strlen( ic->text );
		font_height = get_font_height( ic->gd ) ;
		font_width  = get_font_width( ic->gd ) ;
		maxlen = area.width * 2 / font_width;

		text_line_num = char_total_num / maxlen + 1;
		if( font_height  > ic->area.height || font_width  >ic->area.width)
		{
            EGUI_PRINT_ERROR("font for desktop icon is too large");

        	return -1;
		}

		ic->text_field.x = area.x ;
		ic->text_field.y = area.y + area.height - ( text_line_num * font_height );
		ic->text_field.width = area.width;
		ic->text_field.height = text_line_num * font_height ;

		ic->img_field.x = area.x  ;
		//ic->img_field.x = area.x + area.width/4 ;
		ic->img_field.y = area.y  ;
		ic->img_field.width = area.width ;
		ic->img_field.height = area.height - ic->text_field.height;

	}
	else
	{
		ic->img_field.x = area.x ;
		ic->img_field.y = area.y ;
		ic->img_field.width = area.width ;
		ic->img_field.height = area.height ;
	}
    return 1;
}

/*
 * @breaf private,
 *
 * */

si_t file_exist(char * file_path)
{
	si_t fd;

	if( ( file_path != NULL ) && ( fd = open(file_path , O_RDONLY) )== -1 )
	{
		return 0;
	}
	else
	{
        /* Don't need to close fd as open is not successful */
		return 1;
	}
}



si_t icon_draw_img(struct icon *ic)
{
	set_area(ic->gd, ic->img_field.x , ic->img_field.y, ic->img_field.width  , ic->img_field.height );

	fill_rectangle(ic->gd, ic->img_field.x , ic->img_field.y, ic->img_field.width  , ic->img_field.height );

	set_color(ic->gd, ic->back_color.r, ic->back_color.g, ic->back_color.b, ic->back_color.a);

	if(file_exist(ic->img_path))
	{
		draw_img(ic->gd, ic->img_path, ALIGN_HORIZONTAL_TYPE_CENTER | ALIGN_VERTICAL_TYPE_CENTER );
	}

	return 1;
}

si_t icon_show_text(struct icon * ic)
{

	/* 总字数 ， 单字宽度，单字高度，每行字数 ,总行数,最后一行的字数 */
	si_t char_total_num ;
	si_t font_width;
	si_t font_height;
	si_t maxlen;
	si_t text_line_num;
	si_t last_line_text_num;

	si_t i = 0;


	char_total_num = strlen( ic->text );

	font_width  = get_font_width( ic->gd ) ;

	font_height = get_font_height( ic->gd ) ;

	maxlen = (ic->area.width * 2 )/ font_width;

	last_line_text_num = char_total_num % maxlen ;

	text_line_num = char_total_num / maxlen + 1;

	set_color(ic->gd, ic->fore_color.r, ic->fore_color.g, ic->fore_color.b, ic->fore_color.a);

	/* 设置区域 , 文字区域*/
 	set_area(ic->gd,  ic->text_field.x , ic->text_field.y , ic->text_field.width , ic->text_field.height);

/*	 填充矩形 
	fill_rectangle(ic->gd,  ic->text_field.x, ic->text_field.y, ic->text_field.width , ic->text_field.height);
*  */
	/* 显示文字 */
	for( i = 0; i < text_line_num -1; i++ )
	{
		show_text(ic->gd, ic->text_field.x, ic->text_field.y + i * font_height  , ic->text + i*maxlen ,maxlen);
	}

	show_text(  ic->gd,
			    ic->text_field.x + ( ic->text_field.width  - last_line_text_num * font_width ) / 2,
			    ic->text_field.y + i * font_height  ,
			    ic->text + i * maxlen ,
			    last_line_text_num
			 );
	return 0;

}


si_t
icon_exit
(struct icon * i)
{
    return widget_exit(WIDGET_POINTER(i));
}
void  icon_set_img_path(struct icon *ic, char * img_path)
{
	strcpy(ic->img_path,img_path);
}
char* icon_get_img_path(struct icon *ic)
{
	return ic->img_path;
}

void  icon_set_img_normal_path(struct icon *ic, char * img_path)
{
	strcpy(ic->img_normal_path,img_path);
	return ;
}
char* icon_get_img_normal_path(struct icon *ic)
{
	return ic->img_normal_path;
}
void  icon_set_img_select_path(struct icon *ic, char * img_path)
{
	strcpy(ic->img_select_path,img_path);
	return ;
}
char* icon_get_img_select_path(struct icon *ic)
{
	return ic->img_select_path;
}

void  icon_set_text(struct icon *ic, char * text)
{
	strcpy(ic->text, text);
	return;
}
char* icon_get_text(struct icon *ic)
{
	return ic->text;
}

void  icon_set_type(struct icon *ic, si_t type)
{
	ic->type = type;
	return ;
}

si_t icon_get_type(struct icon *ic)
{
	return ic->type;
}
void  icon_set_is_text_visiable(struct icon *ic, si_t visiable_model)
{
	ic->is_text_visiable = visiable_model;
	return ;
}
si_t icon_get_is_text_visiable(struct icon *ic)
{
	return ic->is_text_visiable;
}


si_t icon_default_callback(void* self , void* msg )
{

    struct icon* ic = self;
    union message * m = msg;
    
    widget_default_callback(ic, m);

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            icon_default_widget_repaint(ic, m);
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_MOUSE_PRESS:
            icon_default_mouse_press(ic, m);
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_MOUSE_RELEASE:
            icon_default_mouse_release(ic, m);
            icon_default_widget_show(ic, m);
            break;

        default:
            break;
    }

    return 0;
}


/* 外部 API */
si_t icon_repaint(struct icon * ic)
{
  	union message msg;

    msg.widget_repaint.area.x = ic->area.x;
    msg.widget_repaint.area.y = ic->area.y;
    msg.widget_repaint.area.width = ic->area.width;
    msg.widget_repaint.area.height = ic->area.height;

    icon_default_widget_repaint(ic, &msg);

    return 0;

}


si_t icon_default_widget_repaint(struct icon* ic , union message * msg)
{
	NOT_USED(msg);
	icon_update_all_areas( ic );

	icon_draw_img( ic );

	if(icon_get_is_text_visiable(ic) == 1)
	{
		icon_show_text(ic);
	}

    return 0;

}

/* 外部API  */
si_t icon_show(struct icon * ic)
{
  	union message msg;

    msg.widget_repaint.area.x = ic->area.x;
    msg.widget_repaint.area.y = ic->area.y;
    msg.widget_repaint.area.width = ic->area.width;
    msg.widget_repaint.area.height = ic->area.height;

    icon_default_widget_show(ic, &msg);

    return 0;

}
/*  */
si_t icon_default_widget_show(struct icon* ic , union message * msg)
{
	struct rectangle area;
	NOT_USED(msg);

    application_widget_absolute_area(WIDGET_POINTER(ic), &area);
	set_area( ic->gd, area.x, area.y, area.width, area.height );
	update( ic->gd );

	return 0;
}

/* 
 * 图标被按下后，应该能够显示四周边框,Color:black，其他的特效，待完成。
 * 因此，需要将画笔的颜色设置为黑色
 *
 **/
si_t icon_default_mouse_press(struct icon* ic , union message * msg)
{
	struct rectangle area;
	NOT_USED(msg);

    application_widget_absolute_area(WIDGET_POINTER(ic), &area);

    /* 设置区域 */
    set_area(ic->gd, area.x, area.y, area.width, area.height);

	/* 上边框 */
    set_color(ic->gd, 0, 0, 0, 0);

    draw_line( ic->gd, area.x, area.y, area.x + area.width - 1, area.y );

	/* 左边框 */
    draw_line( ic->gd, area.x, area.y, area.x, area.y + area.height - 1);

     /* 右边框 */
    draw_line(ic->gd, area.x + area.width - 1, area.y, area.x + area.width - 1, area.y + area.height - 1);

    /* 下边框 */
    draw_line(ic->gd, area.x, area.y + area.height - 1, area.x + area.width - 1, area.y + area.height - 1);

    return 0;

}


void icon_set_bounds(struct icon* icon, si_t x, si_t y, si_t width , si_t height)
{
	icon->area.x = x;
	icon->area.y = y;
	icon->area.width = width;
	icon->area.height = height;
	return ;
}

/* 图标被按下后，应该能够显示四周边框,Color:black，其他的特效，待完成。
 * 因此，需要将画笔的颜色设置为白色
 *
 **/
si_t icon_default_mouse_release(struct icon* ic , union message * msg)
{
	struct rectangle area;
	//NOT_USED(msg);

	application_widget_absolute_area(WIDGET_POINTER(ic), &area);
	
    /* 设置区域 */
    set_area(ic->gd, area.x, area.y, area.width, area.height);

	/* 上边框 */
    /* FIXME: should not be hard code */
    set_color(ic->gd, 255, 255, 255, 0);

	draw_line( ic->gd, area.x, area.y, area.x + area.width - 1, area.y );

	/* 左边框 */
    draw_line( ic->gd, area.x, area.y, area.x, area.y + area.height - 1);

     /* 右边框 */
    draw_line(ic->gd, area.x + area.width - 1,  area.y,  area.x + area.width - 1, area.y + area.height - 1);

    /* 下边框 */
    draw_line(ic->gd, area.x,  area.y + area.height - 1, area.x + area.width - 1, area.y + area.height - 1);

    return 0;

}



