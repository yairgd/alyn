/*
 * =====================================================================================
 *
 *       Filename:  effect.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2023 10:08:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "effect.h"


void effect_render(struct effect_base * e, struct canvas * c, struct rect * r) {
	if (e->ops->render) {
		e->ops->render(e,c,r);
	}

}
void effect_set_config(struct effect_base * e, struct effect_configuration * cd ) {
	if (e->ops->config) {
		e->config_id = cd->id; //config_id;
		e->ops->config(e, cd->data);
	}
}

/**
 * Created  10/06/2023
 * @brief   return the type of the effect class. Each effect class has a unique OPS structure.
 * @note  
 * @param   
 * @return  
 */
struct effect_ops *  effect_get_type(struct effect_base * e) {
	return e->ops;
}





