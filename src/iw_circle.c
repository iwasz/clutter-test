/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <math.h>
#include "iw_circle.h"

/**
 * SECTION:cb-button
 * @short_description: Button widget
 *
 * A button widget with support for a text label and background color.
 */

/* convenience macro for GType implementations; see:
 * http://library.gnome.org/devel/gobject/2.27/gobject-Type-Information.html#G-DEFINE-TYPE:CAPS
 */
G_DEFINE_TYPE (IwCircle, iw_circle, CLUTTER_TYPE_ACTOR);

/* macro for accessing the object's private structure */
#define IW_CIRCLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), IW_TYPE_CIRCLE, IwCirclePrivate))

/* private structure - should only be accessed through the public API;
 * this is used to store member variables whose properties
 * need to be accessible from the implementation; for example, if we
 * intend to create wrapper functions which modify properties on the
 * actors composing an object, we should keep a reference to the actors
 * here
 *
 * this is also the place where other state variables go:
 * for example, you might record the current state of the button
 * (toggled on or off) or a background image
 */
struct _IwCirclePrivate {
        ClutterColor *color;
        ClutterContent *canvas;
};

static gboolean draw_circle (ClutterCanvas *canvas, cairo_t *cr, int width, int height, gpointer *data);
static void on_actor_resize (ClutterActor *actor, const ClutterActorBox *allocation, ClutterAllocationFlags flags, gpointer user_data);
static gboolean idle_resize (gpointer data);

/* from http://mail.gnome.org/archives/gtk-devel-list/2004-July/msg00158.html:
 *
 * "The finalize method finishes releasing the remaining
 * resources just before the object itself will be freed from memory, and
 * therefore it will only be called once. The two step process helps break
 * cyclic references. Both dispose and finalize must chain up to their
 * parent objects by calling their parent's respective methods *after* they
 * have disposed or finalized their own members."
 */
static void iw_circle_finalize (GObject *gobject)
{
        IwCirclePrivate *priv = IW_CIRCLE (gobject)->priv;

        clutter_color_free (priv->color);

        /* call the parent class' finalize() method */
        G_OBJECT_CLASS (iw_circle_parent_class)->finalize (gobject);
}

static void star_actor_pick (ClutterActor *actor, const ClutterColor *pick_color)
{
        if (!clutter_actor_should_pick_paint (actor)) return;

        ClutterActorBox allocation = {
                0,
        };
        gfloat width, height;

        clutter_actor_get_allocation_box (actor, &allocation);
        clutter_actor_box_get_size (&allocation, &width, &height);

        cogl_path_new ();

        cogl_set_source_color4ub (pick_color->red, pick_color->green, pick_color->blue, pick_color->alpha);

        /* create and store a path describing a star */
        //        cogl_path_move_to (width * 0.5, 0);
        //        cogl_path_line_to (width, height * 0.75);
        //        cogl_path_line_to (0, height * 0.75);
        //        cogl_path_move_to (width * 0.5, height);
        //        cogl_path_line_to (0, height * 0.25);
        //        cogl_path_line_to (width, height * 0.25);
        //        cogl_path_line_to (width * 0.5, height);
        cogl_path_ellipse (width / 2.0, height / 2.0, width / 2.0, height / 2.0);
        cogl_path_fill ();
}

/* GObject class and instance initialization functions; note that
 * these have been placed after the Clutter implementation, as
 * they refer to the static function implementations above
 */

/* class init: attach functions to superclasses, define properties
 * and signals
 */
static void iw_circle_class_init (IwCircleClass *klass)
{
        ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
        GParamSpec *pspec;

        gobject_class->finalize = iw_circle_finalize;
//        gobject_class->set_property = iw_circle_set_property;
//        gobject_class->get_property = iw_circle_get_property;

        // It still got destroyed even when I do not override the destroy method (like virtual function in C++).
        //        actor_class->allocate = iw_circle_allocate;
        //        actor_class->paint = iw_circle_paint;
        //        actor_class->paint_node = iw_circle_paint_node;
        actor_class->pick = star_actor_pick;

        g_type_class_add_private (klass, sizeof (IwCirclePrivate));
}

/* object init: create a private structure and pack
 * composed ClutterActors into it
 */
static void iw_circle_init (IwCircle *self)
{
        IwCirclePrivate *priv;
        ClutterLayoutManager *layout;

        priv = self->priv = IW_CIRCLE_GET_PRIVATE (self);

        //        clutter_actor_set_reactive (CLUTTER_ACTOR (self), TRUE);

        /* the only child of this actor is a ClutterBox with a
         * ClutterBinLayout: painting and allocation of the actor basically
         * involves painting and allocating this child box
         */
        //        layout = clutter_bin_layout_new (CLUTTER_BIN_ALIGNMENT_CENTER, CLUTTER_BIN_ALIGNMENT_CENTER);

        //        priv->child = clutter_actor_new ();
        //        clutter_actor_set_layout_manager (priv->child, layout);

        //        /* set the parent of the ClutterBox to this instance */
        //        clutter_actor_add_child (CLUTTER_ACTOR (self), priv->child);

        //        /* add text label to the button; see the ClutterText API docs
        //         * for more information about available properties
        //         */
        //        priv->label = g_object_new (CLUTTER_TYPE_TEXT, "line-alignment", PANGO_ALIGN_CENTER, "ellipsize", PANGO_ELLIPSIZE_END, NULL);

        //        clutter_actor_add_child (priv->child, priv->label);

        //        /* add a ClutterClickAction on this actor, so we can proxy its
        //         * "clicked" signal into a signal from this actor
        //         */
        //        priv->click_action = clutter_click_action_new ();
        //        clutter_actor_add_action (CLUTTER_ACTOR (self), priv->click_action);

        //        g_signal_connect (priv->click_action, "clicked", G_CALLBACK (iw_circle_clicked), NULL);
        priv->color = clutter_color_new (0, 0, 0, 255);

        priv->canvas = clutter_canvas_new ();
        clutter_canvas_set_size (CLUTTER_CANVAS (priv->canvas), 300, 300);
        clutter_actor_set_content (CLUTTER_ACTOR (self), priv->canvas);
        clutter_actor_set_content_scaling_filters (CLUTTER_ACTOR (self), CLUTTER_SCALING_FILTER_TRILINEAR, CLUTTER_SCALING_FILTER_LINEAR);
        g_object_unref (priv->canvas);

        /* connect our drawing code */
        g_signal_connect (priv->canvas, "draw", G_CALLBACK (draw_circle), priv);
        /* invalidate the canvas, so that we can draw before the main loop starts */
        clutter_content_invalidate (priv->canvas);

        g_signal_connect (CLUTTER_ACTOR (self), "allocation-changed", G_CALLBACK (on_actor_resize), NULL);
}

/* public API */
/* examples of public API functions which wrap functions
 * on internal actors
 */

/**
 * iw_circle_set_background_color:
 * @self: a #IwCircle
 * @color: the #ClutterColor to use for the button's background
 *
 * Set the color of the button's background
 */
void iw_circle_set_color (IwCircle *self, const ClutterColor *color)
{
        g_return_if_fail (IW_IS_CIRCLE (self));

        // clutter_actor_set_background_color (self->priv->child, color);
        self->priv->color->alpha = color->alpha;
        self->priv->color->red = color->red;
        self->priv->color->green = color->green;
        self->priv->color->blue = color->blue;

        clutter_content_invalidate (self->priv->canvas);
}

/**
 * iw_circle_new:
 *
 * Creates a new #IwCircle instance
 *
 * Returns: a new #IwCircle
 */
ClutterActor *iw_circle_new (void) { return g_object_new (IW_TYPE_CIRCLE, NULL); }

static gboolean draw_circle (ClutterCanvas *canvas, cairo_t *cr, int width, int height, gpointer *data)
{
        IwCirclePrivate *priv = (IwCirclePrivate *)data;

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
        cairo_set_line_width (cr, 0);

        /* the black rail that holds the seconds indicator */
        clutter_cairo_set_source_color (cr, priv->color);
        cairo_translate (cr, 0.5, 0.5);
        cairo_arc (cr, 0, 0, 0.5, 0, G_PI * 2);
        cairo_fill (cr);

        /* we're done drawing */
        return TRUE;
}

static guint idle_resize_id;

static gboolean idle_resize (gpointer data)
{
        ClutterActor *actor = data;
        float width, height;

        /* match the canvas size to the actor's */
        clutter_actor_get_size (actor, &width, &height);
        clutter_canvas_set_size (CLUTTER_CANVAS (clutter_actor_get_content (actor)), ceilf (width), ceilf (height));

        /* unset the guard */
        idle_resize_id = 0;

        /* remove the timeout */
        return G_SOURCE_REMOVE;
}

static void on_actor_resize (ClutterActor *actor, const ClutterActorBox *allocation, ClutterAllocationFlags flags, gpointer user_data)
{
        /* throttle multiple actor allocations to one canvas resize; we use a guard
         * variable to avoid queueing multiple resize operations
         */
        if (idle_resize_id == 0) {
                idle_resize_id = clutter_threads_add_timeout (1000, idle_resize, actor);
        }
}
