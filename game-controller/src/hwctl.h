/*
 * =====================================================================================
 *
 *       Filename:  hwctl.h
 *
 *    Description: : 
 *
 *        Version:  1.0
 *        Created:  08/14/2023 03:43:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef HWCTL_H
#define HWCTL_H

void hwctl_enable_node(int id);
void hwctl_start_free_run();
void hwctl_stop_free_run();
void hwctl_disable_all_nodes();
void hwctl_manage_blink();
void hwctl_init ();
#endif

