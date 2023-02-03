#ifndef PLOT_X11_H
#define PLOT_X11_H

#include <X11/keysym.h>

typedef struct
{
  unsigned long black;
  unsigned long blue;
  unsigned long green;
  unsigned long cyan;
  unsigned long red;
  unsigned long magenta;
  unsigned long brown;
  unsigned long light_gray;
  unsigned long dark_gray;
  unsigned long light_blue;
  unsigned long light_green;
  unsigned long light_cyan;
  unsigned long yellow;
  unsigned long white;
}
COLORS;

#define BLACK           (rColors.black)
#define BLUE            (rColors.blue)
#define GREEN           (rColors.green)
#define CYAN            (rColors.cyan)
#define RED             (rColors.red)
#define MAGENTA         (rColors.magenta)
#define BROWN           (rColors.brown)
#define LIGHTGRAY       (rColors.light_gray)
#define DARKGRAY        (rColors.dark_gray)
#define LIGHTBLUE       (rColors.light_blue)
#define LIGHTGREEN      (rColors.light_green)
#define LIGHTCYAN       (rColors.light_cyan)
#define YELLOW          (rColors.yellow)
#define WHITE           (rColors.white)

extern COLORS rColors;		/* Цвета */
extern int nWWidth;		/* Текущая ширина экрана */
extern int nWHeight;		/* Текущая высота экрана */

/* Коды, возвращаемые функцией обработки нажатия клавиши */
#define KEY_PRESS_NOTHING	0	/* продолжить далее */
#define KEY_PRESS_EXPOSE	1	/* перерисовать окно */
#define KEY_PRESS_QUIT		(-1)	/* выйти из программы */

/*
 * DrawWindow - работа с окном.
 * Возвращает не 0 в случае ошибки.
 */
int DrawWindow (void (*DrawWindowContent) (),	/* Указатель на функцию, рисующую в окне */
		int (*KeyPressFunction) (int)	/* Указатель на функцию, вызываемую при нажатии на клавишу */
  );

void WSetColor (unsigned long color);
void WDrawString (const char *string, int x, int y);
void WDrawPoint (int x, int y);
void WDrawLine (int x_start, int y_start, int x_end, int y_end);
void WDrawRectangle (int x_top_left, int y_top_left,
		     int x_bottom_right, int y_bottom_right);
void WFillRectangle (int x_top_left, int y_top_left,
		     int x_bottom_right, int y_bottom_right);
void WFillTriangle (int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);

/* Задать тип заполнения многоугольников.
   Style может принимать значения
   FillSolid, FillTiled, FillStippled, FillOpaeueStippled. */
void WSetFillStyle (int Style);

/* Установить битовую карту для заполнения фона. */
void WSetStipple (unsigned int Index);
/* Установить карту пикселов для заполнения фона. */
void WSetTile (unsigned int Index);

/* Выделить цвет по составляющим Red, Green, Blue.
   Возвращает индекс цвета в палитре в Color.
   Возвращаемое значение: 0 - успех, 1 - ошибка. */
int WGetColor (unsigned int Red, unsigned int Green, unsigned int Blue,
	       unsigned long *pColor);

/* Установить заголовок окна */
void WSetTitle (const char *s);

/* Коды ошибки : */
#define PLOT_X11_ERR_1	1
#define PLOT_X11_ERR_2	2
#define PLOT_X11_ERR_3	3
#define PLOT_X11_ERR_4	4
#define PLOT_X11_ERR_5	5

/* Расшифровка кодов ошибки : */
#define PLOT_X11_ERR_MSG_1	"Cannot connect to the X server."
#define PLOT_X11_ERR_MSG_2	"Not enough memory to create X Windows data structures."
#define PLOT_X11_ERR_MSG_3	"Cannot allocate enough colors in the X Windows pallete."
#define PLOT_X11_ERR_MSG_4	"Cannot allocate enough X bitmaps."
#define PLOT_X11_ERR_MSG_5	"Bad Window."
#define PLOT_X11_ERR_MSG_DEF	"Unknown error in an X Windows interface code."

#endif /* PLOT_X11_H */
