#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "plot_x11.h"

#define WND_X           0
#define WND_Y           0
#define WND_WDT         800
#define WND_HGT         600
#define WND_MIN_WDT     50
#define WND_MIN_HGT     50
#define WND_BORDER_WDT  5

#define WND_TITLE       "Test"
#define WND_ICON_TITLE  "Test"
#define PRG_CLASS       "Test"

/*
 * SetWindowManagerHints - процедура передает информацию о свойствах
 * программы менеджеру окон.
 * Возвращает не 0 в случае ошибки.
 */

static int
SetWindowManagerHints (Display * prDisplay,	/* Указатель на структуру Display       */
		       const char *psPrgClass,	/* Класс программы                      */
		       Window nWnd,	/* Идентификатор окна                   */
		       int nMinWidth,	/* Минимальная ширина окна              */
		       int nMinHeight,	/* Минимальная высота окна              */
		       const char *psTitle,	/* Заголовок окна                       */
		       const char *psIconTitle,	/* Заголовок пиктограмы окна            */
		       Pixmap nIconPixmap	/* Рисунок пиктограмы                   */
  )
{
  XSizeHints rSizeHints;	/* Рекомендации о размерах окна         */

  XWMHints rWMHints;
  XClassHint rClassHint;
  XTextProperty prWindowName, prIconName;

  if (!XStringListToTextProperty ((char**)&psTitle, 1, &prWindowName)
      || !XStringListToTextProperty ((char**)&psIconTitle, 1, &prIconName))
    return 1;			/* No memory! */

  rSizeHints.flags = PPosition | PSize | PMinSize;
  rSizeHints.min_width = nMinWidth;
  rSizeHints.min_height = nMinHeight;

  rWMHints.flags = StateHint | IconPixmapHint | InputHint;
  rWMHints.initial_state = NormalState;
  rWMHints.input = True;
  rWMHints.icon_pixmap = nIconPixmap;

  rClassHint.res_name = 0;
  rClassHint.res_class = (char*) psPrgClass;

  XSetWMProperties (prDisplay, nWnd, &prWindowName, &prIconName, 0, 0,
		    &rSizeHints, &rWMHints, &rClassHint);

  return 0;
}

/*
 * Возвращает не 0 в случае ошибки.
 */
static int
AllocColors (Display * prDisplay,	/* Указатель на структуру Display       */
	     Colormap colormap,	/* Палитра                              */
	     COLORS * colors	/* Заполняется функцией                 */
  )
{
  XColor rColor;
  XColor rColorBase;

  if (!XAllocNamedColor (prDisplay, colormap, "black", &rColor, &rColorBase))
    return 1;
  colors->black = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "blue", &rColor, &rColorBase))
    return 2;
  colors->blue = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "green", &rColor, &rColorBase))
    return 3;
  colors->green = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "cyan", &rColor, &rColorBase))
    return 4;
  colors->cyan = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "red", &rColor, &rColorBase))
    return 5;
  colors->red = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "magenta", &rColor, &rColorBase))
    return 6;
  colors->magenta = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "brown", &rColor, &rColorBase))
    return 7;
  colors->brown = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "light gray", &rColor, &rColorBase))
    return 8;
  colors->light_gray = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "dark gray", &rColor, &rColorBase))
    return 9;
  colors->dark_gray = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "light blue", &rColor, &rColorBase))
    return 10;
  colors->light_blue = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "light green", &rColor, &rColorBase))
    return 11;
  colors->light_green = rColor.pixel;

  if (!XAllocNamedColor
      (prDisplay, colormap, "light cyan", &rColor, &rColorBase))
    return 12;
  colors->light_cyan = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "yellow", &rColor, &rColorBase))
    return 13;
  colors->yellow = rColor.pixel;

  if (!XAllocNamedColor (prDisplay, colormap, "white", &rColor, &rColorBase))
    return 14;
  colors->white = rColor.pixel;

  return 0;
}

/*
 * Возвращает не 0 в случае ошибки.
 */
static int
CreateBitmaps (Display * prDisplay,	/* Указатель на структуру Display       */
	       Drawable nWnd,	/* Идентификатор окна                   */
	       Pixmap * prPixmap	/* Битовые карты, заполняется функцией  */
  )
{
  int i;
  char psBitmapBits[17][8] = {
    /* 0 */
    {
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff			/*  11111111b */
     },
    /* 1 */
    {
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xee,			/*  11101110b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xff,			/*  11111111b */
     0xee			/*  11101110b */
     },
    /* 2 */
    {
     0xff,			/*  11111111b */
     0xbb,			/*  10111011b */
     0xff,			/*  11111111b */
     0xee,			/*  11101110b */
     0xff,			/*  11111111b */
     0xbb,			/*  10111011b */
     0xff,			/*  11111111b */
     0xee			/*  11101110b */
     },
    /* 3 */
    {
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xee,			/*  11101110b */
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xee			/*  11101110b */
     },
    /* 4 */
    {
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xaa			/*  10101010b */
     },
    /* 5 */
    {
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa,			/*  10101010b */
     0xff,			/*  11111111b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa			/*  10101010b */
     },
    /* 6 */
    {
     0xdd,			/*  11011101b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa			/*  10101010b */
     },
    /* 7 */
    {
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0xdd,			/*  11011101b */
     0xaa			/*  10101010b */
     },
    /* 8 */
    {
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0xaa			/*  10101010b */
     },
    /* 9 */
    {
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55,			/*  01010101b */
     0xaa,			/*  10101010b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55			/*  01010101b */
     },
    /* 10 */
    {
     0x22,			/*  00100010b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55			/*  01010101b */
     },
    /* 11 */
    {
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x22,			/*  00100010b */
     0x55			/*  01010101b */
     },
    /* 12 */
    {
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x55			/*  01010101b */
     },
    /* 13 */
    {
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x11,			/*  00010001b */
     0x00,			/*  00000000b */
     0x55,			/*  01010101b */
     0x00,			/*  00000000b */
     0x11			/*  00010001b */
     },
    /* 14 */
    {
     0x00,			/*  00000000b */
     0x44,			/*  01000100b */
     0x00,			/*  00000000b */
     0x11,			/*  00010001b */
     0x00,			/*  00000000b */
     0x44,			/*  01000100b */
     0x00,			/*  00000000b */
     0x11			/*  00010001b */
     },
    /* 15 */
    {
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x11,			/*  00010001b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x11			/*  00010001b */
     },
    /* 16 */
    {
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00,			/*  00000000b */
     0x00			/*  00000000b */
     }
  };

  for (i = 0; i <= 16; i++)
    if (!(prPixmap[i] = XCreateBitmapFromData (prDisplay, nWnd,
					       psBitmapBits[i], 8, 8)))
      return i + 1;
  return 0;
}

static void
FreeBitmaps (Display * prDisplay,	/* Указатель на структуру Display       */
	     Pixmap * prPixmap	/* Битовые карты, освобождаются функцией */
  )
{
  int i;

  for (i = 0; i <= 16; i++)
    XFreePixmap (prDisplay, prPixmap[i]);
}

/* Структуры данных, используемые функциями в данном файле.                  */
static Display *prDisplay;	/* Указатель на структуру Display       */
static int nScreenNum;		/* Номер экрана                         */
static GC prGC;
static Window nWnd;		/* Идентификатор окна                   */
static Drawable draw;		/* Текущий объект рисования		*/
static Pixmap prPixmap[17];	/* Битовые карты                        */

/* Структуры данных, используемые функциями и вне данного файла.             */
COLORS rColors;			/* Цвета */
int nWWidth = WND_WDT;		/* Текущая ширина экрана */
int nWHeight = WND_HGT;		/* Текущая высота экрана */


/*
 * DrawWindow - работа с окном.
 * Возвращает не 0 в случае ошибки.
 */
int
DrawWindow (void (*DrawWindowContent) (),	/* Указатель на функцию, рисующую в окне*/
	    int (*KeyPressFunction) (int)	/* Указатель на функцию, вызываемую при нажатии на клавишу */
  )
{
  XEvent rEvent;
  XWindowAttributes rAttributes;
  KeySym nKeySym;
  int nDepth;

  /* Устанавливаем связь с сервером */
  if ((prDisplay = XOpenDisplay (NULL)) == NULL)
    return PLOT_X11_ERR_1;	/* Cannot connect to the X server. */

  /* Получаем номер основного экрана */
  nScreenNum = DefaultScreen (prDisplay);

  /* Создаем окно. */
  nWnd = XCreateSimpleWindow (prDisplay,
			      RootWindow (prDisplay, nScreenNum),
			      WND_X, WND_Y, WND_WDT, WND_HGT, WND_BORDER_WDT,
			      BlackPixel (prDisplay, nScreenNum),
			      WhitePixel (prDisplay, nScreenNum));

  /* Задаем рекомендации для менеджера окон */
  if (SetWindowManagerHints (prDisplay, PRG_CLASS, nWnd,
			     WND_MIN_WDT, WND_MIN_HGT, WND_TITLE,
			     WND_ICON_TITLE, 0))
    return PLOT_X11_ERR_2;	/* Not enough memory. */

  /* Выбираем события, обрабатываемые программой. */
  XSelectInput (prDisplay, nWnd, ExposureMask | KeyPressMask);

  /* Показываем окно */
  XMapWindow (prDisplay, nWnd);

  prGC = XCreateGC (prDisplay, nWnd, 0, NULL);
  if (AllocColors
      (prDisplay, DefaultColormap (prDisplay, nScreenNum), &rColors))
    return PLOT_X11_ERR_3; /* Cannot allocate pallete. */

  if (CreateBitmaps (prDisplay, nWnd, prPixmap))
    return PLOT_X11_ERR_4; /* Cannot allocate bitmaps. */

  /* Размеры окна */
  if (XGetWindowAttributes (prDisplay, nWnd, &rAttributes))
    {
      nWWidth = rAttributes.width;
      nWHeight = rAttributes.height;
      nDepth = rAttributes.depth;
    }
  else
    return PLOT_X11_ERR_5;	/* Bad window */

  /* Устанавливаем текущий объект рисования */
  draw = XCreatePixmap (prDisplay, nWnd, nWWidth, nWHeight, nDepth);
  if (!draw)
    return PLOT_X11_ERR_4;	/* Cannot allocate bitmaps. */

  /* Цикл получения и обработки событий */
  for (;;)
    {
      XNextEvent (prDisplay, &rEvent);

      switch (rEvent.type)
	{
	case Expose:
	  /* Запрос на перерисовку */
	  if (rEvent.xexpose.count != 0)
	    break;

	  if (XGetWindowAttributes (prDisplay, nWnd, &rAttributes))
	    {
	      if (nWWidth != rAttributes.width
		  || nWHeight != rAttributes.height
		  || nDepth != rAttributes.depth)
		{
		  /* Размеры окна изменились. Удаляем старый объект
		     рисования и создаем новый */
		  XFreePixmap (prDisplay, draw);
		  nWWidth = rAttributes.width;
		  nWHeight = rAttributes.height;
		  nDepth = rAttributes.depth;
		  draw = XCreatePixmap (prDisplay, nWnd, nWWidth, nWHeight, nDepth);
		  if (!draw)
		    return PLOT_X11_ERR_4;	/* Cannot allocate bitmaps. */
		}
	    }
	  else
	    return PLOT_X11_ERR_5;	/* Bad window */

	  DrawWindowContent ();
	  XCopyArea (prDisplay, draw, nWnd, prGC, 0, 0,
		     nWWidth, nWHeight, 0, 0);

	  break;

	case KeyPress:
	  /* Нажатие клавиши клавиатуры */
	  XLookupString (&rEvent.xkey, NULL, 0, &nKeySym, NULL);

	  /* Вызываем функцию обработки нажатия на клавиши */
	  switch (KeyPressFunction (nKeySym))
	    {
	    case KEY_PRESS_NOTHING:
	      break;

	    case KEY_PRESS_EXPOSE:
	      /* Посылаем сообщение о необходимости перерисовки */
	      rEvent.type = Expose;
	      rEvent.xexpose.count = 0;
	      XSendEvent (prDisplay, nWnd, True, 0, &rEvent);
	      break;

	    default:	/* если неизвестный код, то завершаем работу */
	    case KEY_PRESS_QUIT:
	      XFreePixmap (prDisplay, draw);
	      FreeBitmaps (prDisplay, prPixmap);
	      XFreeGC (prDisplay, prGC);
	      XCloseDisplay (prDisplay);
	      return 0;
	    }
	  break;
	}
      /*  Пропускаем остальные события */
      while (XCheckMaskEvent(prDisplay, KeyPressMask | KeyReleaseMask, &rEvent));
    }

  return 0;
}

void
WSetColor (unsigned long color)
{
  XSetForeground (prDisplay, prGC, color);
}

void
WDrawString (const char *string, int x, int y)
{
  XDrawString (prDisplay, draw, prGC, x, y, string, strlen (string));
}

void
WDrawPoint (int x, int y)
{
  XDrawPoint (prDisplay, draw, prGC, x, y);
}

void
WDrawLine (int x_start, int y_start, int x_end, int y_end)
{
  XDrawLine (prDisplay, draw, prGC, x_start, y_start, x_end, y_end);
}

void
WDrawRectangle (int x_top_left, int y_top_left,
		int x_bottom_right, int y_bottom_right)
{
  XDrawRectangle (prDisplay, draw, prGC, x_top_left, y_top_left,
		  x_bottom_right - x_top_left + 1,
		  y_bottom_right - y_top_left + 1);
}

void
WFillRectangle (int x_top_left, int y_top_left,
		int x_bottom_right, int y_bottom_right)
{
  XFillRectangle (prDisplay, draw, prGC, x_top_left, y_top_left,
		  x_bottom_right - x_top_left + 2,
		  y_bottom_right - y_top_left + 2);
}

void
WFillTriangle (int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
  XPoint points[3] = { {x_1, y_1}, {x_2, y_2}, {x_3, y_3} };
  XFillPolygon (prDisplay, draw, prGC, points, 3, Convex, CoordModeOrigin);
}

void
WFillPoligon (XPoint * points, int num)
{
  XFillPolygon (prDisplay, draw, prGC, points, num, Convex, CoordModeOrigin);
}

/* Задать тип заполнения многоугольников.
   Style может принимать значения
   FillSolid, FillTiled, FillStippled, FillOpaeueStippled. */
void
WSetFillStyle (int Style)
{
  XSetFillStyle (prDisplay, prGC, Style);
}

/* Установить битовую карту для заполнения фона. */
void
WSetStipple (unsigned int Index)
{
  XSetStipple (prDisplay, prGC, prPixmap[Index]);
}

/* Установить карту пикселов для заполнения фона. */
void
WSetTile (unsigned int Index)
{
  XSetTile (prDisplay, prGC, prPixmap[Index]);
}

/* Выделить цвет по составляющим Red, Green, Blue.
   Возвращает индекс цвета в палитре в Color.
   Возвращаемое значение: 0 - успех, 1 - ошибка. */
int
WGetColor (unsigned int Red, unsigned int Green, unsigned int Blue,
	   unsigned long *pColor)
{
  XColor rColor;

  rColor.red = Red << 8;
  rColor.green = Green << 8;
  rColor.blue = Blue << 8;
  rColor.flags = DoRed | DoGreen | DoBlue;

  if (XAllocColor (prDisplay, DefaultColormap (prDisplay, nScreenNum), &rColor) == 0)	/* ошибка */
    return 1;

  *pColor = rColor.pixel;
  return 0;
}

/* Установить заголовок окна */
void WSetTitle (const char *s)
{
  XTextProperty tProp;
  XStringListToTextProperty ((char**)&s, 1, &tProp);
  XSetWMName (prDisplay, nWnd, &tProp);
}
