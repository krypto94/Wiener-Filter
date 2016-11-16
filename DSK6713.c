#include "Configuration1cfg.h"
#include "dsk6713.h"
#include "dsk6713_aic23.h"

float filter_Coeff[] = {1.8898,0.8409,-0.1082,-0.3522,-0.1641,-0.3569,0.3119,0.963,0.6665,-0.2226,-0.1886,-0.5160,-0.58,-0.0723,0.54,0.4327,0.0510,-0.3012,-0.3988,-0.2277};


static short in_buffer[100]; 


DSK6713_AIC23_Config config = { \
    0x0017,  /* 0 DSK6713_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK6713_AIC23_RIGHTINVOL Right line input channel volume */\
    0x00d8,  /* 2 DSK6713_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x00d8,  /* 3 DSK6713_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0011,  /* 4 DSK6713_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK6713_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK6713_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK6713_AIC23_DIGIF      Digital audio interface format */ \
    0x0081,  /* 8 DSK6713_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK6713_AIC23_DIGACT     Digital interface activation */   \
};


/*
 *  main() - Main code routine, initializes BSL and generates tone
 */

void main()
{
    DSK6713_AIC23_CodecHandle hCodec;
  
    Uint32 l_input, r_input,l_output, r_output;
    
    /* Initialize the board support library, must be called first */
    DSK6713_init();
     
    /* Start the codec */
    hCodec = DSK6713_AIC23_openCodec(0, &config);
    	
    DSK6713_AIC23_setFreq(hCodec, 1);
    
       while(1)
        {	/* Read a sample to the left channel */
			while (!DSK6713_AIC23_read(hCodec, &l_input));
			
			/* Read a sample to the right channel */
			while (!DSK6713_AIC23_read(hCodec, &r_input));          
				
				l_output=l_input;
 		 	    r_output=l_output;
			
			/* Send a sample to the left channel */
            while (!DSK6713_AIC23_write(hCodec, l_output));

            /* Send a sample to the right channel */
            while (!DSK6713_AIC23_write(hCodec, r_output));
        }   
    /* Close the codec */
    DSK6713_AIC23_closeCodec(hCodec);
}


signed int FIR_FILTER(float * h, signed int x)
{
int i=0;
signed long output=0;

in_buffer[0] = x;  /* new input at buffer[0]  */

for(i=30;i>0;i--)
in_buffer[i] = in_buffer[i-1]; /* shuffle the buffer   */

for(i=0;i<32;i++)
output = output + h[i] * in_buffer[i];

return(output);

}



