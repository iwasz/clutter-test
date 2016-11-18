/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <clutter/clutter.h>
#include <iostream>
#include "cb_button.h"

/* colors */
static const ClutterColor stage_color = { 0x33, 0x33, 0x55, 0xff };
static const ClutterColor white_color = { 0xff, 0xff, 0xff, 0xff };
static const ClutterColor yellow_color = { 0x88, 0x88, 0x00, 0xff };

void on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data);
void clicked_cb (ClutterClickAction *action, ClutterActor *actor, gpointer user_data);
void on_paint (ClutterActor *actor, gpointer user_data);

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
                ClutterColor actor_color = { 0, 0, 255, 128 };
                ClutterActor *rect = clutter_actor_new ();
                clutter_actor_set_background_color (rect, &actor_color);
                clutter_actor_set_size (rect, 100, 100);
                clutter_actor_set_position (rect, 300, 300);
                clutter_actor_add_child (CLUTTER_ACTOR (stage), rect);
                clutter_actor_set_reactive (rect, TRUE);

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

        ClutterActor *button = cb_button_new ();
        cb_button_set_text (CB_BUTTON (button), "hello");
        /* note that the size of the button is left to Clutter's size requisition */
        cb_button_set_text_color (CB_BUTTON (button), &white_color);
        cb_button_set_background_color (CB_BUTTON (button), &yellow_color);
        clutter_actor_add_child (stage, button);

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

void on_paint (ClutterActor *actor, gpointer user_data)
{
        ClutterActorBox allocation = {
                0,
        };
        gfloat width, height;
        guint tmp_alpha;

        /* priv is a private internal struct */
        //        ClutterColor color = STAR_ACTOR (actor)->priv->color;

        clutter_actor_get_allocation_box (actor, &allocation);
        clutter_actor_box_get_size (&allocation, &width, &height);

        //        tmp_alpha = clutter_actor_get_paint_opacity (actor) * color.alpha / 255;

        cogl_path_new ();

        //        cogl_set_source_color4ub (color.red, color.green, color.blue, tmp_alpha);
        cogl_set_source_color4ub (0, 0, 255, 128);

        /* create and store a path describing a star */
        cogl_path_move_to (width * 0.5, 0);
        cogl_path_line_to (width, height * 0.75);
        cogl_path_line_to (0, height * 0.75);
        cogl_path_move_to (width * 0.5, height);
        cogl_path_line_to (0, height * 0.25);
        cogl_path_line_to (width, height * 0.25);
        cogl_path_line_to (width * 0.5, height);

        cogl_path_fill ();
        g_signal_stop_emission_by_name (actor, "paint");
}
