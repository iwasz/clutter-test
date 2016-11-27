/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include <iostream>
#include <cmath>
#include "cb_button.h"
#include "iw_circle.h"
#include "iw_line.h"
#include "iw_circular_node.h"

/* colors */
static const ClutterColor stage_color = { 0x33, 0x33, 0x55, 0xff };
static const ClutterColor white_color = { 0xff, 0xff, 0xff, 0xff };
static const ClutterColor yellow_color = { 0x88, 0x88, 0x00, 0xff };

void on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data);
void clicked_cb (ClutterClickAction *action, ClutterActor *actor, gpointer user_data);
void on_paint (ClutterActor *actor, gpointer user_data);
void iw_circle_destroy (ClutterActor *actor, gpointer user_data);

/*****************************************************************************/
int main (int argc, char **argv)
{
        //        if (clutter_init (&argc, &argv) < 1) {
        //                std::cerr << "clutter_init failed" << std::endl;
        //        }

        if (gtk_clutter_init_with_args (&argc, &argv, NULL, NULL, NULL, NULL) != CLUTTER_INIT_SUCCESS) {
                g_error ("Unable to initialize GtkClutter");
        }

        GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_maximize (GTK_WINDOW (window));
        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

        GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add (GTK_CONTAINER (window), vbox);

        GtkWidget *button = gtk_button_new_with_label("test");
        gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

        GtkWidget *clutter = gtk_clutter_embed_new ();
        gtk_widget_set_size_request (clutter, 512, 512);
        gtk_box_pack_start (GTK_BOX (vbox), clutter, TRUE, TRUE, 0);

        //        ClutterActor *stage = clutter_stage_new ();
        ClutterActor *stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter));

        ClutterColor stage_color = { 236, 236, 236, 255 };
        // clutter_actor_set_size (stage, 512, 512);
        clutter_actor_set_background_color (CLUTTER_ACTOR (stage), &stage_color);
        //        g_signal_connect (stage, "destroy", G_CALLBACK (clutter_main_quit), NULL);
        gtk_widget_show_all (window);

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
                clutter_actor_set_reactive (rect, TRUE);
                clutter_actor_add_child (CLUTTER_ACTOR (stage), rect);

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
                clutter_actor_set_position (circle, 200, 400);
                ClutterColor actor_color = { 0, 150, 198, 201 };
                iw_circle_set_color (IW_CIRCLE (circle), &actor_color);
                //        g_signal_connect (circle, "destroy", G_CALLBACK (iw_circle_destroy), NULL);
                clutter_actor_set_reactive (circle, TRUE);

                clutter_actor_add_child (stage, circle);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (circle, dragAction);
        }
        /*---------------------------------------------------------------------------*/
        {
                ClutterActor *line = iw_line_new ();
                clutter_actor_set_size (line, 100, 100);
                clutter_actor_set_position (line, 300, 100);
                ClutterColor actor_color = { 0, 150, 198, 201 };
                iw_line_set_color (IW_LINE (line), &actor_color);
                clutter_actor_set_reactive (line, TRUE);
                clutter_actor_add_child (stage, line);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (line, dragAction);
        }
        /*---------------------------------------------------------------------------*/
        {
                ClutterActor *circularNode = iw_circular_node_new ();
                clutter_actor_set_position (circularNode, 100, 300);
                iw_circular_node_set_radius (IW_CIRCULAR_NODE (circularNode), 75);
                ClutterColor actor_color = { 0, 150, 198, 201 };
                iw_circular_node_set_color (IW_CIRCULAR_NODE (circularNode), &actor_color);
                clutter_actor_set_reactive (circularNode, TRUE);

                iw_circular_node_set_ports_no (IW_CIRCULAR_NODE (circularNode), 3);
                iw_circular_node_set_port_angle (IW_CIRCULAR_NODE (circularNode), 0, G_PI - 0.5);
                iw_circular_node_set_port_size (IW_CIRCULAR_NODE (circularNode), 0, 0.4);
                ClutterColor port_color = { 141, 141, 141, 255 };
                iw_circular_node_set_port_color (IW_CIRCULAR_NODE (circularNode), 0, &port_color);

                iw_circular_node_set_port_angle (IW_CIRCULAR_NODE (circularNode), 1, G_PI + 0.5);
                iw_circular_node_set_port_size (IW_CIRCULAR_NODE (circularNode), 1, 0.4);
                iw_circular_node_set_port_color (IW_CIRCULAR_NODE (circularNode), 1, &port_color);

                iw_circular_node_set_port_angle (IW_CIRCULAR_NODE (circularNode), 2, 2 * G_PI);
                iw_circular_node_set_port_size (IW_CIRCULAR_NODE (circularNode), 2, 0.4);
                ClutterColor port_color2 = { 209, 209, 209, 255 };
                iw_circular_node_set_port_color (IW_CIRCULAR_NODE (circularNode), 2, &port_color2);

                clutter_actor_add_child (stage, circularNode);

                ClutterAction *dragAction = clutter_drag_action_new ();
                clutter_actor_add_action (circularNode, dragAction);
        }
        /*---------------------------------------------------------------------------*/
        clutter_actor_show (stage);

        gtk_main ();
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
