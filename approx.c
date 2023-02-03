/*
#include <stdio.h>
#include <malloc.h>
#include <math.h>
*/

typedef double FPT; // floating point type for further usage

void rearrange_poly3(FPT x1, FPT x_n, FPT *coeffs) {
/*
converts (coeffs of the) polynomial with respect to (x-x1)
to polynomial with respect to (x-x_n)
*/
	FPT c0, c1, c2, c3;
	c0=coeffs[0]; c1=coeffs[1];
	c2=coeffs[2]; c3=coeffs[3];
	coeffs[0]=c0+(x_n-x1)*((c3*(x_n-x1)+c2)*(x_n-x1)+c1);
	coeffs[1]=(3*c3*(x_n-x1)+2*c2)*(x_n-x1)+c1;
	coeffs[2]=c2+3*c3*(x_n-x1);
	coeffs[3]=c3;
	return ;
}


void poly_3val_der(FPT x1, FPT x2, FPT x3,
				   FPT f1, FPT f2, FPT f3, FPT d, FPT *coeffs) {
/*
Makes coeffs[j] = j'th (at (x-x_min)^j) coeff of the cubic polynomial f(x)
	  such that f(x_i)=f_i, (df/dx)|(x=x_1)=d;
	  here x_min=min(x1, x2, x3)
*/
	FPT x_min;
	coeffs[0]=f1;
	coeffs[1]=d;
	coeffs[2]=((f2-f1)/(x2-x1)-d)/(x2-x1);
	coeffs[3]=(((f3-f2)/(x3-x2)-(f2-f1)/(x2-x1))/(x3-x1)-coeffs[2])/(x3-x1);
	// converting to form: sum(c[j]*(x-x1)^j)
	coeffs[2]=coeffs[2]+coeffs[3]*(x1-x2);

	// converting to form: sum(c[j]*(x-x_min)^j)
	if (x1>x2 || x1>x3) {
		x_min=x2>x3 ? x3 : x2;
		rearrange_poly3(x1, x_min, coeffs);
	}
	return ;
}

void Bessel_approx(int n, FPT *x, FPT *f, FPT *coeffs) {
/*
Constructs Bessel cubic polynomial approx. of f(x).
On return, coeffs[j..j+3] contains coeffs of the
approx. polynomial at [x[j]; x[j+1]], j=0..n-2
*/
	int j;
	FPT d1, d2, dx1, dx2, dx3, df1, df2, df3;

	dx1=x[2]-x[1];
	dx2=x[3]-x[2];
	dx3=x[4]-x[3];
	df1=f[2]-f[1];
	df2=f[3]-f[2];
	df3=f[4]-f[3];

	// 1st polynomial (at [x0; x2])
	d1=(dx2*df1/dx1+dx1*df2/dx2)/(dx2+dx1); d2=0;
	poly_3val_der(x[2], x[0], x[1],
				  f[2], f[0], f[1], d1, coeffs);
	for (j=0; j<=3; j++)
		coeffs[j+4]=coeffs[j];
	rearrange_poly3(x[0], x[1], coeffs+4);

	for (j=1; j<=n-5; j++) {
		d1=(dx2*df1/dx1+dx1*df2/dx2)/(dx2+dx1);
		d2=(dx3*df2/dx2+dx2*df3/dx3)/(dx3+dx2);

		coeffs[4*(j+1)]=f[j+1];
		coeffs[4*(j+1)+1]=d1;
		coeffs[4*(j+1)+2]=(3*df2/dx2-2*d1-d2)/dx2;
		coeffs[4*(j+1)+3]=(d1+d2-2*df2/dx2)/dx2/dx2;

		dx1=dx2; dx2=dx3;
		dx3=x[j+3]-x[j+2];
		df1=df2; df2=df3;
		df3=f[j+3]-f[j+2];
	}
	//Now d2=Bessel approx. to f'(x[n-3])

	// last polynomial (at [x[n-3]; x[n-1]])
	poly_3val_der(x[n-3], x[n-2], x[n-1],
				  f[n-3], f[n-2], f[n-1], d2, coeffs+4*(n-3));
	for (j=4*(n-3); j<=4*(n-3)+3; j++)
		coeffs[j+4]=coeffs[j];
	rearrange_poly3(x[n-3], x[n-2], coeffs+4*(n-2));

	return ;
}

void spline_approx(int n, FPT *x, FPT *f, FPT *coeffs) {
/*
Constructs cubic-spline polynomial approx. of f(x).
On return, coeffs[j..j+3] contains coeffs of the
approx. polynomial at [x[j]; x[j+1]], j=0..n-2
*/
	int j;
	FPT *d, *l, *u, *r, div_dif;
	
	if ((d = (FPT *) malloc(n*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate d in spline_approx\n");
	   return ;  // terminate program if out of memory 
	}
	if ((l = (FPT *) malloc((n-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate l in spline_approx\n");
	   return ;  // terminate program if out of memory 
	}
        if ((u = (FPT *) malloc((n-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate u in spline_approx\n");
	   return ;  // terminate program if out of memory 
	}
	if ((r = (FPT *) malloc(n*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate r in spline_approx\n");
	   return ;  // terminate program if out of memory 
	}
	
	/* preparing 3-diag lin. sys. vectors (look Bogachev, pp. 108, 114-115)   
	
	   d[0] u[0]      ...             | r[0]
	   l[0] d[1] u[1]                   ... 
	   ...             ...    u[n-2]  | r[n-2] 
	                   l[n-2] d[n-1]  | r[n-1]  
	   
	  NB!!! Now d[j] denotes coefficient at the d_j variable,
	        later (after solving) d_j value will be placed in it
	*/		
	
	d[0]=x[2]-x[1];
	u[0]=x[2]-x[0];
	r[0]=((f[1]-f[0])/(x[1]-x[0])*(x[2]-x[1])*(2*x[2]+x[1]-3*x[0])+
	      (f[2]-f[1])/(x[2]-x[1])*(x[1]-x[0])*(x[1]-x[0]))/(x[2]-x[0]);
	      
        for (j=1; j<=n-2; j++) {
	    d[j]=2*(x[j+1]-x[j-1]);
	    l[j-1]=x[j-1]-x[j];
	    u[j]=x[j]-x[j-1];
	    r[j]=3*((f[j]-f[j-1])/(x[j]-x[j-1])*(x[j+1]-x[j])+
	            (f[j+1]-f[j])/(x[j+1]-x[j])*(x[j]-x[j-1]));	    
	}	      
	      
        l[n-2]=x[n-1]-x[n-3];
	d[n-1]=x[n-2]-x[n-3];
	r[n-1]=((f[n-2]-f[n-3])/(x[n-2]-x[n-3])*(x[n-1]-x[n-2])*(x[n-1]-x[n-2])+
	      (f[n-1]-f[n-2])/(x[n-1]-x[n-2])*(x[n-2]-x[n-3])*
	      (3*x[n-1]-x[n-2]-2*x[n-3]))/(x[n-1]-x[n-3]);	   
	      
        // Solving the obtained 3-diag. lin. sys.
	
	for (j=0; j<=n-2; j++) {
	    d[j+1]-=u[j]*l[j]/d[j];
	    r[j+1]-=r[j]*l[j]/d[j];
	}
	
	d[n-1]=r[n-1]/d[n-1];
	for (j=n-2; j>=0; j--) {
	    d[j]=(r[j]-d[j+1]*u[j])/d[j];	    
	}	 	       
	
	// Now d[j] contains d_j - approximations to derivatives
	// It's time to construct approx. polynomials	  
	
	for (j=0; j<=n-2; j++) {
	   div_dif=(f[j+1]-f[j])/(x[j+1]-x[j]);
           coeffs[4*j]=f[j];
	   coeffs[4*j+1]=d[j];
	   coeffs[4*j+2]=(3*div_dif-2*d[j]-d[j+1])/(x[j+1]-x[j]);
	   coeffs[4*j+3]=(d[j]+d[j+1]-2*div_dif)/(x[j+1]-x[j])/(x[j+1]-x[j]);
	}
	 
	free(d); free(r);
	free(l); free(u);

	return ;
}

FPT eval_cubic_approx(FPT x0, int n, FPT *x, FPT *coeffs) {
/*
if x \in [x[j]; x[j+1]] (j=0..n-2) returns
the appr. polynomial (with coeffs[j..j+3] coefficients)
value at x=x0
*/
  int j, f=0;

  for (j=0; j<=n-2; j++) {
	if (x[j]<=x0 && x[j+1]>x0) {
		f=1;
		break;
	}
  }
  if (fabs(x0-x[n-1])<0.00001) {
	j=n-2;
	f=1;
  }

  if (f)
	return ((coeffs[4*j+3]*(x0-x[j])+
			 coeffs[4*j+2])*(x0-x[j])+
			 coeffs[4*j+1])*(x0-x[j])+
			 coeffs[4*j];
  else {
	printf("Error in eval_cubic_approx : x value, %5.2f, out of interval (%5.2f;%5.2f)\n", x0, x[0], x[n-1]);
	return 0;
  }
}

/*

FPT fun(FPT x) {

	return sin(x);

}



int main() {

	int n, j, k;
	FPT a, b, z, y, *x, *f, *coeffs;

	printf("Points number: ");
	scanf("%d", &n);
	printf("2 boundary points: ");
	scanf("%g %g", &a, &b);

	if ((x = (FPT *) malloc(n*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate x values\n");
	   return 1;  // terminate program if out of memory 
	}
	if ((f = (FPT *) malloc(n*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate f(x) values\n");
	   return 1;  // terminate program if out of memory 
	}
	if ((coeffs = (FPT *) malloc(4*(n-1)*sizeof(FPT))) == NULL)
	{
	   printf("Not enough memory to allocate approx. polynomials\n");
	   return 1;  // terminate program if out of memory 
	}

	printf("\n");
	for (j=0; j<=n-1; j++) {
		x[j]=a+(b-a)*j/(n-1);
		f[j]=fun(x[j]);
		printf("%10.5f %10.5f\n", x[j], f[j]);
	}

	//poly_3val_der(7., 5., 2., 2144., 752., 44., 950., coeffs);

	Bessel_approx(n, x, f, coeffs);

	printf("Values:\n");
	for (j=0; j<=n-2; j++) {
		//x[j]=a+(b-a)*j/(n-1);
		z=(x[j]+x[j+1])/2;
		y=fun(z)-eval_cubic_approx(z, n, x, coeffs);
		printf("%10.5f %10.5f\n", z, y);
	}

	free(x);
	free(f);
	free(coeffs);

	return 0;
}

*/
