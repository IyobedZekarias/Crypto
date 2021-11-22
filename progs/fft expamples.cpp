#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<time.h>
#include<fftw3.h>


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage:\n\t[1]Number of equidistant points\n");
		return 1;
	}
	int Npointspol = atoi(argv[1]);
	int Npoints = 2 * Npointspol + 1;
	int i;
	fftwl_complex *in, *out;
	fftwl_complex *in1, *out1;
	fftwl_complex *in2, *out2;
	fftwl_plan planback;
	fftwl_plan planfor;

	in = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);			//pay attention
	out = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);		//pay attention
	in1 = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);		//pay attention
	out1 = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);		//pay attention
	in2 = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);		//pay attention
	out2 = (fftwl_complex*) fftwl_malloc(sizeof(fftwl_complex)*Npoints);		//pay attention
	planback = fftwl_plan_dft_1d(Npoints, in, out, FFTW_BACKWARD, FFTW_ESTIMATE); 	//Here we set which kind of transformation we want to perform
	planfor = fftwl_plan_dft_1d(Npoints, in, out, FFTW_FORWARD, FFTW_ESTIMATE); 	//Here we set which kind of transformation we want to perform

	srand(time(NULL));
	for(i = 0; i < Npointspol; i++)
	{
		in1[i][0] = static_cast<long double>(rand()%(10));
		in2[i][0] = rand()%(10); 
	}
	printf("Polynomial  coefficients:\n");
	for(i = 0; i < Npointspol; i++)
	{
		printf("%2d %11.7f %11.7f\n", i, creal(in1[i][0]), creal(in2[i][0]));
	}
	for(i = Npointspol; i < Npoints; i++)
	{
		in1[i][0] = 0.;
		in2[i][0] = 0.;
	}

	for(i = 0; i < Npoints; i++)
	{
		in[i][0] = in1[i][0];
    //in[i][1] = in1[i][1]; 
	}
	fftwl_execute(planback);								//Execution of FFT
	for(i = 0; i < Npoints; i++)
	{
		out1[i][0] = out[i][0];
    //out1[i][1] = out[i][1]; 
		in[i][0] = in2[i][0];
    //in[i][1] = in2[i][1]; 
	}
	fftwl_execute(planback); 							//Execution of FFT
	for(i = 0; i < Npoints; i++)
	{
		out2[i][0] = out[i][0];
    //out2[i][1] = out[i][1]; 
	}

	for(i = 0; i < Npoints; i++)
	{
		//in[i][1] = out1[i][1]*out2[i][1];
    in[i][0] = out1[i][0]*out2[i][0]; 
	}
	fftwl_execute(planfor); 								//Execution of FFT

	printf("\nPolynomial coefficients of the product:\n");
	for(i = 0; i < Npoints; i++)
	{
		printf("%2d %11.7f\n", i, creal(out[i][0])/(double)Npoints);
	}
	
	fftwl_destroy_plan(planback);							//Free memory
	fftwl_destroy_plan(planfor);							//Free memory
	fftwl_free(in);									//Free memory
	fftwl_free(in1);									//Free memory
	fftwl_free(in2);									//Free memory
	fftwl_free(out);									//Free memory
	fftwl_free(out1);								//Free memory
	fftwl_free(out2);								//Free memory
	fftwl_cleanup();									//Free all the remaining memory
	return 0;
}