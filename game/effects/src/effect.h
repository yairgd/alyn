/*
 * =====================================================================================
 *
 *       Filename:  effect.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2023 10:07:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef EFFETC_H
#define EFFETC_H 
#include "canvas.h"

#ifdef __cplusplus
extern "C" {
#endif
	struct effect_parameters  {
		char varname[32];
		union valdue {
			double dv;
			int iv;
			struct rect r;
			char text[256];
		} value;

	};
	struct effect_configuration  {
		int id;
		void * data;
	};

	struct effect_base {
		int x,y; 		 // top left to take from  recnagle data
		struct rect r;		 // the x,y and width, height of the object when it placed in its parent
		struct canvas * canvas;  // pointer to canvas - it passes fom the parent of the object
		void * object_data;      // implementation data of the effect
		struct effect_ops * ops; 
		int config_id;
	};

	struct effect_ops {
		void (*render)(struct effect_base * , struct canvas * canvas, struct  rect * r);
		void (*config)(struct effect_base *, void * data);
	};



	void effect_render(struct effect_base * e, struct canvas * c, struct rect * r);
	void effect_set_config(struct effect_base * e, struct effect_configuration  * cd );
	struct effect_parameters *  effect_get_parameters(struct effect_base * e);

	struct effect_ops *  effect_get_type(struct effect_base * e);

#ifdef __cplusplus
}
#endif

#endif
