pkglib_LTLIBRARIES += library/libwidget.la

library_libwidget_la_SOURCES = %D%/widget/widget.c\
					   %D%/button/button.c\
					   %D%/label/label.c\
					   %D%/panel/panel.c\
					   %D%/scroll_bar/scroll_bar.c\
					   %D%/text_line/text_line.c\
					   %D%/image_view/image_view.c\
					   %D%/window/window.c\
					   %D%/window/paint_window_decoration.c\
					   %D%/dialog/dialog.c\
					   %D%/icon/icon.c

noinst_HEADERS += %D%/window/paint_window_decoration.h

nobase_pkginclude_HEADERS += %D%/widget/widget.h\
							 %D%/button/button.h\
							 %D%/label/label.h\
							 %D%/panel/panel.h\
							 %D%/scroll_bar/scroll_bar.h\
							 %D%/text_line/text_line.h\
							 %D%/image_view/image_view.h\
							 %D%/window/window.h\
							 %D%/dialog/dialog.h\
							 %D%/icon/icon.h\
							 %D%/external.h

library_libwidget_la_LIBADD = library/libclient_lib.la library/libgeometry.la library/libcomm.la \
					  library/libconfig_parser.la library/libdata_structures.la

library_libwidget_la_LDFLAGS = -version-info 1:0:0

# %D%/window/window_status.h is not complete, thus not distributing now.