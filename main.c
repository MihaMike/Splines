#include <stdio.h>
#include <math.h>
#include "plot_x11.h"
#include <malloc.h>
#include <approx.c>

/* Вариант рисуемого графика */
static int variant = 0,
       N=300, n_appr = 50;
            
static double a=-3.14, b=3.14, y_min, y_max,  
              *x_val, *f_val, *coeffs1, *coeffs2;
	      
double fun(double z) {
 return z+sin(z)+cos(z)*cos(z) ;
}	      
	      
double fun_Bessel(double z) {
    return eval_cubic_approx(z, n_appr, x_val, coeffs1);
}

double err_Bessel(double z) {
    return eval_cubic_approx(z, n_appr, x_val, coeffs1)-fun(z);
}

double fun_spline(double z) {
    return eval_cubic_approx(z, n_appr, x_val, coeffs2);
}

double err_spline(double z) {
    return eval_cubic_approx(z, n_appr, x_val, coeffs2)-fun(z);
}	      

void plot_fun(double (*fun)(double z), double a, double b, int N)  {
      int j, x_c, y_c, width=nWWidth, height=nWHeight;      
      double *x, *f, max_val, min_val, u_x, u_y,
             x_start, y_start, x_end, y_end;      
      
      x = (double *)malloc(N*sizeof(double));
      f = (double *)malloc(N*sizeof(double));
      
      max_val=-100000000;
      min_val=100000000;      
      for (j=0; j<=N-1; j++) { 
        x[j]=a+(b-a)*j/(N-1);     
        f[j]=(*fun)(x[j]);
	if (f[j]>max_val) 
	  max_val=f[j];
	if (f[j]<min_val)
	  min_val=f[j];    
      }	
      
      u_x=width/(b-a);      
      u_y=(height*9./10.)/(max_val-min_val);     
      x_c=width/2; 
      y_c=height/2;            
      
      x_start=floor((x[0]-a)*u_x);
      y_start=height*19./20.-floor((f[0]-min_val)*u_y);
      for (j=0; j<=N-2; j++) {
        x_end=floor((x[j+1]-a)*u_x);
	y_end=height*19./20.-floor((f[j+1]-min_val)*u_y);
	WDrawLine(x_start, y_start, x_end, y_end);
	x_start=x_end;
	y_start=y_end;	
      }            
      
      free(x);
      free(f);    
      
      y_min=min_val;
      y_max=max_val;     
      
      return;  
    }             


void plot_2fun(double (*fun1)(double z), double (*fun2)(double z), 
               double a, double b, int N)  {
      int j, x_c, y_c, width=nWWidth, height=nWHeight;      
      double *x, *f1, *f2, max_val, min_val, u_x, u_y,
             x_start, x_end, y_start1, y_end1, y_start2, y_end2;      
      
      x = (double *)malloc(N*sizeof(double));
      f1 = (double *)malloc(N*sizeof(double));
      f2 = (double *)malloc(N*sizeof(double));      
      
      max_val=-100000000;
      min_val=100000000;      
      for (j=0; j<=N-1; j++) { 
        x[j]=a+(b-a)*j/(N-1);     
        f1[j]=(*fun1)(x[j]);
	f2[j]=(*fun2)(x[j]);
	if (f1[j]>max_val) 
	  max_val=f1[j];
	if (f1[j]<min_val)
	  min_val=f1[j];
	if (f2[j]>max_val) 
	  max_val=f2[j];
	if (f2[j]<min_val)
	  min_val=f2[j];      
      }      
      
      u_x=width/(b-a);      
      u_y=(height*9./10.)/(max_val-min_val);     
      x_c=width/2; 
      y_c=height/2; 
   
      
      x_start=floor((x[0]-a)*u_x);
      WSetColor(GREEN);
      y_start1=height*19./20.-floor((f1[0]-min_val)*u_y);
      WSetColor(MAGENTA);
      y_start2=height*19./20.-floor((f2[0]-min_val)*u_y);
      for (j=0; j<=N-2; j++) {
        x_end=floor((x[j+1]-a)*u_x);
	y_end1=height*19./20.-floor((f1[j+1]-min_val)*u_y);
	y_end2=height*19./20.-floor((f2[j+1]-min_val)*u_y);
	WSetColor(GREEN);
	WDrawLine(x_start, y_start1, x_end, y_end1);
	WSetColor(MAGENTA);
	WDrawLine(x_start, y_start2, x_end, y_end2);
	x_start=x_end;
	y_start1=y_end1;	
	y_start2=y_end2;
      }            
      
      free(x);
      free(f1);
      free(f2);
      
      y_min=min_val;
      y_max=max_val;          
      
      return;  
    }
    
void draw_axes_and_labels() {
  int width = nWWidth,		/* Текущая ширина экрана */
      height = nWHeight,	/* Текущая высота экрана */
      j;
      
  char *s;
  
  WSetColor(BLACK);
  for (j=1; j<=5; j++) {
	WDrawLine(j/6.*width, height/20., j/6.*width, height*19./20.);
	WDrawLine(0, height/20.+9./10.*height*j/6. , width, 
	             height/20.+9./10.*height*j/6.);
  }
  WDrawString("X", 59./60.*width, 16./30.*height);
  WDrawString("Y", 31./60.*width, 1./17.*height);
    
  for (j=1; j<=5; j++) {
        s=(char *) malloc(50);
	sprintf(s, "%6.3f", a+(b-a)*j/6.);
	WDrawString(s, width*(10*j+1)/60., height*21./40.);
	free(s);    
  }
  for (j=1; j<=5; j++) {
        s=(char *) malloc(50);
	sprintf(s, "%6.3f", y_min+(y_max-y_min)*j/6.);
	WDrawString(s, width*71./140, height*(19./20.-9./10.*(5*j+1)/30.));
	free(s);    
  } 
  
  return;
}                 

/* Нарисовать содержимое окна */
static void
DrawWindowContent ()
{
  int width = nWWidth,		/* Текущая ширина экрана */
      height = nWHeight;	/* Текущая высота экрана */
      
  WSetColor (DARKGRAY);
  WFillRectangle (0, 0, width, height);

//  WSetColor (BLACK);
//  WDrawLine (0, height / 2, width, height / 2);

  switch (variant)
  {
  case 0:
    WSetTitle("Plotbuilder");
    WSetColor(BLACK);
    WDrawString ("Press Q to quit, 1...7 to view plots,", 10, 20);
    WDrawString ("A/D to shift plotting segment left/right,", 10, 30);
    WDrawString ("S/W for central scale zoom/unzoom,", 10, 40);
    WDrawString ("H to set segment to [-10, 10], F1 for this screen", 10, 50);
    break;
    
  case 1:
    WSetColor(GREEN);
    plot_fun(fun, a, b, N);
    draw_axes_and_labels();    
    break;    
    
  case 2:
    WSetColor (MAGENTA);
    plot_fun(fun_Bessel, a, b, N); 
    draw_axes_and_labels();
    break;
    
  case 3:
    WSetColor (MAGENTA);
    plot_fun(fun_spline, a, b, N); 
    draw_axes_and_labels();
    break;   
    
  case 4:
    plot_2fun(fun, fun_Bessel, a, b, N); 
    draw_axes_and_labels();
    break;
    
  case 5:
    plot_2fun(fun, fun_spline, a, b, N); 
    draw_axes_and_labels();
    break;    

  case 6:
    WSetColor (BLUE);
    plot_fun(err_Bessel, a, b, N); 
    draw_axes_and_labels();
    break;   

  case 7:
    WSetColor (BLUE);
    plot_fun(err_spline, a, b, N); 
    draw_axes_and_labels();
    break;    
  }
}

int KeyPressFunction (int nKeySym)
{
  int j;
  double shift=fabs(b-a)/50.;
  switch (nKeySym)
  {
  case XK_Q:
  case XK_q:
    return KEY_PRESS_QUIT;

  case XK_F1:
    variant = 0;
    WSetTitle ("Plotbuilder");
    break;
  
  case XK_1:
    variant = 1;
    WSetTitle ("Original function");
    break;
  
  case XK_2:
    variant = 2;
    WSetTitle ("Approximation by Bessel cubic polynomials");
    break;
    
  case XK_3:
    variant = 3;
    WSetTitle ("Approximation by cubic splines");
    break;
    
  case XK_4:
    variant = 4;
    WSetTitle ("Function (green) and Bessel approx. (magenta)");
    break;
    
  case XK_5:
    variant=5;
    WSetTitle("Function (green) and spline-approx. (magenta)");
    break;
    
  case XK_6:
    variant=6;
    WSetTitle("Absolute error of Bessel approx.");
    break;
    
  case XK_7:
    variant=7;
    WSetTitle("Absolute error of spline-approx.");
    break;
    
  case XK_a:
    a=a-shift;
    b=b-shift;
    for (j=0; j<=n_appr-1; j++) {
	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;
  case XK_d:
    a=a+shift;
    b=b+shift;
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
     }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;  
  case XK_w:
    a=a-shift;
    b=b+shift;
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }    
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;    
  case XK_s:      
    a=a+shift;
    b=b-shift;
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);    
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;
  case XK_h:
    a=-10;
    b=10;
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;
  case XK_n:
  free(x_val); free(f_val); free(coeffs1); free(coeffs2);
  n_appr=2*n_appr;

  if ((x_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate x values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((f_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate f(x) values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs1 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs2 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
        {
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
  
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;
  case XK_m:
  free(x_val); free(f_val); free(coeffs1); free(coeffs2);
  n_appr=n_appr/2 ;

  if ((x_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate x values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((f_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate f(x) values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs1 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs2 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
        {
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
  
    for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
    }
    Bessel_approx(n_appr, x_val, f_val, coeffs1);
    spline_approx(n_appr, x_val, f_val, coeffs2);
    break;  
        
          
  default:
    return KEY_PRESS_NOTHING;
  }

  /* Перерисовать окно */
  return KEY_PRESS_EXPOSE;
}

int
main ()
{
  int j, ret_code;
  
  if ((x_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate x values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((f_val = (FPT *) malloc(n_appr*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate f(x) values\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs1 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
  if ((coeffs2 = (FPT *) malloc(4*(n_appr-1)*sizeof(FPT))) == NULL)
        {
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}
 	

  for (j=0; j<=n_appr-1; j++) {
  	x_val[j]=a+(b-a)*j/(n_appr-1);
	f_val[j]=fun(x_val[j]);
  }


  Bessel_approx(n_appr, x_val, f_val, coeffs1);
  spline_approx(n_appr, x_val, f_val, coeffs2);

  /* Вывод на экран X11. */
  ret_code = DrawWindow (DrawWindowContent, KeyPressFunction);
  if (ret_code)
    {
      switch (ret_code)
	{
	case PLOT_X11_ERR_1:
	  printf ("%s\n", PLOT_X11_ERR_MSG_1);
	  break;
	case PLOT_X11_ERR_2:
	  printf ("%s\n", PLOT_X11_ERR_MSG_2);
	  break;
	case PLOT_X11_ERR_3:
	  printf ("%s\n", PLOT_X11_ERR_MSG_3);
	  break;
	case PLOT_X11_ERR_4:
	  printf ("%s\n", PLOT_X11_ERR_MSG_4);
	  break;
	case PLOT_X11_ERR_5:
	  printf ("%s\n", PLOT_X11_ERR_MSG_5);
	  break;
	default:
	  printf ("%s\n", PLOT_X11_ERR_MSG_DEF);
	  break;
	}
      return ret_code;
    }

  return 0;
}
