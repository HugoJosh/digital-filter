#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(void)
{
    // calculo de numero de muestras
    int fs = 8000;
    int N = fs*5;                 // tasa de muestreo * tiempo de muestreo
    float f = 300;                  // frecuencia del tono
    float w0 = 2 * 3.1416 * f / fs; // frecuencia digital
    float b1 = sin(w0);
    float a1 = 2 * cos(w0);
    float *osc, *data;//salida sin filtrar y filtrada
    osc = (float*) malloc(N * sizeof(float)); // reservar memoria para salida 
    data = (float*) malloc(N * sizeof(float));

    // coeficienes filtro IIR
    float ciirty[3] = {1,-1.94169223448620,0.996863331833438};
    float ciirtx[3] = {0.998431665916719,-1.94169223448620,0.998431665916719};

    float *salfil, *salsfil;//salida sin filtrar y filtrada
    salfil = (float*) malloc(N * sizeof(float)); // reservar memoria para salida 
    salsfil = (float*) malloc(N * sizeof(float));
 
  // if memory cannot be allocated
  if((salfil == NULL)||(salsfil == NULL)){
    printf("Error! memory not allocated.");
    exit(0);
  }

    //creacion de apuntadores de archivos
    FILE *flee1,*flee2,*flee3,*flee32,*flee4,*flee42,*flee5,*flee52; // apuntadores de salida
    FILE *inicio;
    //asignacion de apuntadores de archivos
    inicio = fopen("voz.dat", "r");
    flee1 = fopen("salfinal.dat", "w");
    flee2 = fopen("saltono.dat", "w");
    flee3 = fopen("espsis1.dat", "w");//mag
    flee32 = fopen("espsis2.dat", "w");//fase
    flee4 = fopen("espin.dat", "w");
    flee42 = fopen("espin2.dat", "w");
    flee5 = fopen("espout.dat", "w");
    flee52 = fopen("espout2.dat", "w");

    // calcula valores del oscilador
    *(osc) = b1;
    *(osc+1) = a1 * b1;
    for (int i = 2; i < N; i++)
    {
        *(osc+i) = a1 * (*(osc+i-1)) - (*(osc+i-2));
    }

    float r=0;
    // mapeo de datos de la voz a un arreglo
    for (int i = 0; i < N; i++)
    {
            fscanf(inicio, "%f",&r);
            *(data+i) = (float)r;
    }
    fclose(inicio);
    inicio=fopen("voz2.dat","w");
    float sum=0;
    for (int i = 0; i < N; i++)
    {
        fprintf(inicio,"%f \t %6.6f \n",((float)i)/fs,data[i]);
        sum+=data[i]*data[i];//Calcula el valor rms de la señal
    }
    fclose(inicio);

    sum=sqrt(sum/N);
    float coefr=sum/(10^(5/20))*sqrt(2);//Calcula el valor pico del ruido
    printf("%f \n",coefr);
    // pondera el oscilador de acuerdo al valor pico calculado
    for (int i = 0; i < N; i++)
    {
        *(osc+i) = coefr * (*(osc+i));
    }

    //Creacion de señal con ruido
    for (int i = 0; i < N; i++)
    {
        *(salsfil+i) = *(data+i) + *(osc+i);            // se suma la señal y el ruido
        fprintf(flee2,  "%6.6f \t %6.6f \n",((float)i)/fs, *(salsfil+i)); // Guardado de datos de salida señal sin filtrar
    }
    fclose(flee2);

    // calcula valores iniciales de la salida filtrada
    int i = 0;
    *(salfil)= ciirtx[0] * (*(salsfil));

    i = 1;
    *(salfil+1) = ciirtx[0] * (*(salsfil+i)) + ciirtx[1] * (*(salsfil+i-1)) - ciirty[1] * (*(salfil+i-1));

    i = 2;
    *(salfil+2) = ciirtx[0] * (*(salsfil+i)) + ciirtx[1] * (*(salsfil+i-1)) + ciirtx[2] * (*(salsfil+i-2)) - ciirty[1] * (*(salfil+i-1)) - ciirty[2] * (*(salfil+i-2));

    // i = 3;
    // *(salfil+3) = ciirtx[0] * (*(salsfil+i)) + ciirtx[1] * (*(salsfil+i-1)) + ciirtx[2] * (*(salsfil+i-2)) + ciirtx[3] * (*(salsfil+i-3)) - ciirty[1] * (*(salfil+i-1)) - ciirty[2] * salfil[i - 2] - ciirty[3] * (*(salfil+i-3));

    // proceso de filtrado
    for (int i = 3; i < N; i++)
    {
        *(salfil+i) = ciirtx[0] * (*(salsfil+i)) + ciirtx[1] * (*(salsfil+i-1)) + ciirtx[2] * (*(salsfil+i-2)) - ciirty[1] * (*(salfil+i-1)) - ciirty[2] * (*(salfil+i-2));
    }
    //se guarda la salida filtrada
    for (int i = 0; i < N; i++)
    {
    fprintf(flee1, "%6.6f\t %6.6f \n",((float)i)/fs,*(salfil+i)); // Guardado de datos de salida señal filtrada
    }
    fclose(flee1);

    //se calcula el espectro del filtro
    //es de 4000 por ser la mitad de la frecuencia de muestreo
    float dw =2* M_PI / (fs/2);
    float hmag[fs/2];
    float hfas[fs/2];
    float hr,hi,rn,rd,id,in;
    for (int w = 0; w < (fs/2); w++)
    {
        hmag[w]=0;
        hfas[w]=0;
        rn=ciirtx[0]+ciirtx[1]*cos(w*dw)+ciirtx[2]*cos(2*w*dw);
        in=-ciirtx[1]*sin(w*dw)-ciirtx[2]*sin(2*w*dw);
        rd= ciirty[0]+ciirty[1]*cos(w*dw)+ciirty[2]*cos(2*w*dw);
        id=-ciirty[1]*sin(w*dw)-ciirty[2]*sin(2*w*dw);
        hr=(rn*rd+in*id)/(rd*rd+id*id);
        hi=(-rn*id+in*rd)/(rd*rd+id*id);
        hmag[w]=sqrt(hr*hr+hi*hi);
        fprintf(flee3, "%6.6f \t %6.6f  \n", w*2*M_PI/(fs/2),hmag[w]); // Guardado de datos del espectro del sistema
        hfas[w]=atan(hi/hr);
        fprintf(flee32, "%6.6f \t %6.6f  \n",w*2*M_PI/(fs/2),hfas[w]);//Guarda datos de la fase del filtro
    }

    //se calcula el espectro de la entrada con la DFT con Goertzel
    float *xr,*xi,*hmag2,*hfas2;
    xr = (float*) malloc(fs * sizeof(float));
    xi = (float*) malloc(fs * sizeof(float));
    hmag2 = (float*) malloc(fs * sizeof(float));
    hfas2 = (float*) malloc(fs * sizeof(float));
    float vk=0;
    float v1=0,v2=0,yr=0,yi=0;
    for (int k = 0; k < fs; k++)
    {   
        v1=0;
        v2=0;
        for (int n = 0; n < 5*fs; n++)
        {   
            vk=(*(salsfil+n))+v1*2*cos(2*5*M_PI*(k)/N) -v2;
            v2=v1;
            v1=vk;
            //*(xr+k)//+=(*(salsfil+n))*cos(2*M_PI*k*n/fs);
            //*(xi+k)//+=(*(salsfil+n))*(-1)*sin(2*M_PI*k*n/fs);
        }
        yr = v2 - v2*cos(2*M_PI*(k)/fs);
        yi = v1*sin(2*M_PI*(k)/fs);
        hmag2[k]=sqrt(yr*yr+yi*yi);//sqrt((*(xi+k))*(*(xi+k))+(*(xr+k))*(*(xr+k)));
        fprintf(flee4, "%6.6f \n",hmag2[k]); // Guardado de datos del espectro de lal entrada
        hfas2[k]=atan(yr/yi);//atan((*(xi+k))/(*(xr+k)));
        fprintf(flee42, "%6.6f \n",hfas2[k]);//Guarda datos de la fase de la entrada
    }
    fclose(flee4);
    fclose(flee42);

    //se calcula el espectro de la salida con la DFT con Goertzel
    //se limpian y reasignan los apuntadores para eliminar los datos previos
    free(xr);
    free(xi);
    free(hmag2);
    free(hfas2);
    xr = (float*) malloc(fs * sizeof(float));
    xi = (float*) malloc(fs * sizeof(float));
    hmag2 = (float*) malloc(fs * sizeof(float));
    hfas2 = (float*) malloc(fs * sizeof(float));
    for (int k = 0; k < fs; k++){
        v1=0;
        v2=0;
        for (int n = 0; n < 5*fs; n++)
        {   
            vk=(*(salfil+n))+v1*2*cos(2*5*M_PI*(k)/N) -v2;
            v2=v1;
            v1=vk;
            //*(xr+k)//+=(*(salsfil+n))*cos(2*M_PI*k*n/fs);
            //*(xi+k)//+=(*(salsfil+n))*(-1)*sin(2*M_PI*k*n/fs);
        }
        yr = v2 - v2*cos(2*M_PI*(k)/fs);
        yi = v1*sin(2*M_PI*(k)/fs);
        hmag2[k]=sqrt(yr*yr+yi*yi);//sqrt((*(xi+k))*(*(xi+k))+(*(xr+k))*(*(xr+k)));
        fprintf(flee5, "%6.6f \n", hmag2[k]); // Guardado de datos del espectro de lal entrada
        hfas2[k]=atan(yr/yi);//atan((*(xi+k))/(*(xr+k)));
        fprintf(flee52, "%6.6f \n", hfas2[k]);//Guarda datos de la fase de la entrada
    }
    fclose(flee5);
    fclose(flee52);

    //se libera la memoria usada en los arrays
    free(salfil);
    free(salsfil);
    free(osc);
    free(data);
    free(xr);
    free(xi);
    free(hmag2);
    free(hfas2);
    //imprime cada una de las graficas
system("gnuplot -p grafgnuplot.gp ");
system("gnuplot -p grafgnuplot22.gp ");
system("gnuplot -p grafgnuplot23.gp ");
system("gnuplot -p grafgnuplot2.gp ");
system("gnuplot -p grafgnuplot3.gp ");
system("gnuplot -p grafgnuplot4.gp ");
system("gnuplot -p grafgnuplot6.gp ");
system("gnuplot -p grafgnuplot5.gp ");
}