/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <clutter/clutter.h>
#include <iostream>
#include <cmath>
#include "cb_button.h"
#include "iw_circle.h"

/* colors */
static const ClutterColor stage_color = { 0x33, 0x33, 0x55, 0xff };
static const ClutterColor white_color = { 0xff, 0xff, 0xff, 0xff };
static const ClutterColor yellow_color = { 0x88, 0x88, 0x00, 0xff };

void on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data);
void clicked_cb (ClutterClickAction *action, ClutterActor *actor, gpointer user_data);
void on_paint (ClutterActor *actor, gpointer user_data);
void iw_circle_destroy (ClutterActor *actor, gpointer user_data);
static gboolean draw_clock (ClutterCanvas *canvas, cairo_t *cr, int width, int height);

/*****************************************************************************/

int main (int argc, char **argv)
{
        if (clutter_init (&argc, &argv) < 1) {
                std::cerr << "clutter_init failed" << std::endl;
        }

        ClutterColor stage_color = { 236, 236, 236, 255 };

        ClutterActor *stage = clutter_stage_new ();
        clutter_actor_set_size (stage, 512, 512);
        clutter_actor_set_background_color (CLUTTER_ACTOR (stage), &stage_color);
        g_signal_connect (stage, "destroy", G_CALLBACK (clutter_main_quit), NULL);

        /*---------------------------------------------------------------------------*/

        {
                ClutterColor actor_color = { 0, 255, 0, 128 };
                ClutterActor *rect = clutter_actor_new ();
                clutter_actor_set_background_color (rect, &actor_color);
                clutter_actor_set_size (rect, 100, 100);
                clutter_actor_set_position (rect, 100, 100);
                clutter_actor_add_child (CLUTTER_ACTOR (stage), rect);
                clutter_actor_set_reactive (rect, TRUE);

                //        ClutterAction *clickAction = clutter_click_action_new ();
                //        clutter_actor_add_action (rect, clickAction);
                //        g_signal_connect (clickAction, "clicked", G_CALLBACK (clicked_cb), NULL);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (rect, dragAction);
                //        g_signal_connect (clickAction, "clicked", G_CALLBACK (clicked_cb), NULL);

                //        g_signal_connect (stage, "button-press-event", G_CALLBACK (on_stage_button_press), NULL);
        }

        /*---------------------------------------------------------------------------*/

        {
                //                ClutterColor actor_color = { 0, 0, 255, 128 };
                ClutterActor *rect = clutter_actor_new ();
                //                clutter_actor_set_background_color (rect, &actor_color);
                                clutter_actor_set_size (rect, 100, 100);
                //                clutter_actor_set_position (rect, 300, 300);
                clutter_actor_set_reactive (rect, TRUE);

                ClutterContent *canvas;
                canvas = clutter_canvas_new ();
                clutter_canvas_set_size (CLUTTER_CANVAS (canvas), 300, 300);
                clutter_actor_set_content (rect, canvas);
                clutter_actor_set_content_scaling_filters (rect, CLUTTER_SCALING_FILTER_TRILINEAR, CLUTTER_SCALING_FILTER_LINEAR);
                clutter_actor_add_child (CLUTTER_ACTOR (stage), rect);
                g_object_unref (canvas);
                /* connect our drawing code */
                g_signal_connect (canvas, "draw", G_CALLBACK (draw_clock), NULL);
                /* invalidate the canvas, so that we can draw before the main loop starts */
                clutter_content_invalidate (canvas);

                //        ClutterAction *clickAction = clutter_click_action_new ();
                //        clutter_actor_add_action (rect, clickAction);
                //        g_signal_connect (clickAction, "clicked", G_CALLBACK (clicked_cb), NULL);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (rect, dragAction);
                //        g_signal_connect (clickAction, "clicked", G_CALLBACK (clicked_cb), NULL);

                //                clutter_actor_show (rect);
                //        g_signal_connect (stage, "button-press-event", G_CALLBACK (on_stage_button_press), NULL);
        }

        /*---------------------------------------------------------------------------*/

        //        ClutterActor *button = cb_button_new ();
        //        cb_button_set_text (CB_BUTTON (button), "hello");
        //        /* note that the size of the button is left to Clutter's size requisition */
        //        cb_button_set_text_color (CB_BUTTON (button), &white_color);
        //        cb_button_set_background_color (CB_BUTTON (button), &yellow_color);
        //        clutter_actor_add_child (stage, button);

        /*---------------------------------------------------------------------------*/
        {
                ClutterActor *circle = iw_circle_new ();
                /* note that the size of the button is left to Clutter's size requisition */
                //        clutter_actor_set_background_color (circle, &yellow_color);
                clutter_actor_set_size (circle, 100, 100);
                //        g_signal_connect (circle, "destroy", G_CALLBACK (iw_circle_destroy), NULL);
                clutter_actor_set_reactive (circle, TRUE);

                clutter_actor_add_child (stage, circle);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (circle, dragAction);
        }
        /*---------------------------------------------------------------------------*/

        clutter_actor_show (stage);

        clutter_main ();
}

/*****************************************************************************/

// void on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data)
//{
//        // find out which part of the screen was clicked
//        gfloat x = 0;
//        gfloat y = 0;
//        clutter_event_get_coords (event, &x, &y);

//        // find which actor was clicked
//        ClutterActor *clicked = clutter_stage_get_actor_at_pos (CLUTTER_STAGE (stage), CLUTTER_PICK_ALL, x, y);

//        // ignore clicks on the stage
//        if (clicked == CLUTTER_ACTOR (stage)) return;

//        // hide the actor that was clicked
//        clutter_actor_hide (clicked);
//}

void clicked_cb (ClutterClickAction *action, ClutterActor *actor, gpointer user_data)
{
        /* display the number of the clicked button (equivalent
         * to the number returned by clutter_event_get_button())
         */
        std::cerr << "Button " << clutter_click_action_get_button (action) << " clicked" << std::endl;
}

void iw_circle_destroy (ClutterActor *actor, gpointer user_data) { std::cerr << "CircleDestroyed" << std::endl; }

static gboolean draw_clock (ClutterCanvas *canvas, cairo_t *cr, int width, int height)
{
        GDateTime *now;
        float hours, minutes, seconds;
        ClutterColor color;

        /* get the current time and compute the angles */
        now = g_date_time_new_now_local ();
        seconds = g_date_time_get_second (now) * G_PI / 30;
        minutes = g_date_time_get_minute (now) * G_PI / 30;
        hours = g_date_time_get_hour (now) * G_PI / 6;

        cairo_save (cr);

        /* clear the contents of the canvas, to avoid painting
         * over the previous frame
         */
        cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
        cairo_paint (cr);

        cairo_restore (cr);

        cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

        /* scale the modelview to the size of the surface */
        cairo_scale (cr, width, height);

        cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_width (cr, 0.1);

        /* the black rail that holds the seconds indicator */
        clutter_cairo_set_source_color (cr, CLUTTER_COLOR_Black);
        cairo_translate (cr, 0.5, 0.5);
        cairo_arc (cr, 0, 0, 0.4, 0, G_PI * 2);
        cairo_stroke (cr);

        /* the seconds indicator */
        color = *CLUTTER_COLOR_White;
        color.alpha = 128;
        clutter_cairo_set_source_color (cr, &color);
        cairo_move_to (cr, 0, 0);
        cairo_arc (cr, sinf (seconds) * 0.4, -cosf (seconds) * 0.4, 0.05, 0, G_PI * 2);
        cairo_fill (cr);

        /* the minutes hand */
        color = *CLUTTER_COLOR_DarkChameleon;
        color.alpha = 196;
        clutter_cairo_set_source_color (cr, &color);
        cairo_move_to (cr, 0, 0);
        cairo_line_to (cr, sinf (minutes) * 0.4, -cosf (minutes) * 0.4);
        cairo_stroke (cr);

        /* the hours hand */
        cairo_move_to (cr, 0, 0);
        cairo_line_to (cr, sinf (hours) * 0.2, -cosf (hours) * 0.2);
        cairo_stroke (cr);

        g_date_time_unref (now);

        /* we're done drawing */
        return TRUE;
}
