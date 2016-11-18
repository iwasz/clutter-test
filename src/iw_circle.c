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

/* enumerates property identifiers for this class;
 * note that property identifiers should be non-zero integers,
 * so we add an unused PROP_0 to occupy the 0 position in the enum
 */
enum { PROP_0, PROP_TEXT };

/* enumerates signal identifiers for this class;
 * LAST_SIGNAL is not used as a signal identifier, but is instead
 * used to delineate the size of the cache array for signals (see below)
 */
enum { CLICKED, LAST_SIGNAL };

/* cache array for signals */
static guint iw_circle_signals[LAST_SIGNAL] = {
        0,
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

/* enables objects to be uniformly treated as GObjects;
 * also exposes properties so they become scriptable, e.g.
 * through ClutterScript
 */
static void iw_circle_set_property (GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
        IwCircle *button = IW_CIRCLE (gobject);

        switch (prop_id) {
        //        case PROP_TEXT:
        //                iw_circle_set_text (button, g_value_get_string (value));
        //                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
                break;
        }
}

/* enables objects to be uniformly treated as GObjects */
static void iw_circle_get_property (GObject *gobject, guint prop_id, GValue *value, GParamSpec *pspec)
{
        IwCirclePrivate *priv = IW_CIRCLE (gobject)->priv;

        switch (prop_id) {
        //        case PROP_TEXT:
        //                g_value_set_string (value, priv->text);
        //                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
                break;
        }
}

/* ClutterActor implementation
 *
 * we only implement destroy(), get_preferred_height(), get_preferred_width(),
 * allocate(), and paint(), as this is the minimum we can get away with
 */

/* composite actors should implement destroy(), and inside their
 * implementation destroy any actors they are composed from;
 * in this case, we just destroy the child ClutterBox
 */
// static void iw_circle_destroy (ClutterActor *self)
//{
//        IwCirclePrivate *priv = IW_CIRCLE (self)->priv;

//        /* we just destroy the child, and let the child
//         * deal with destroying _its_ children; note that we have a guard
//         * here in case the child has already been destroyed
//         */
//        if (priv->child) {
//                clutter_actor_destroy (priv->child);
//                priv->child = NULL;
//        }

//        /* chain up to destroy() on the parent ClutterActorClass;
//         * note that we check the parent class has a destroy() implementation
//         * before calling it
//         */
//        if (CLUTTER_ACTOR_CLASS (iw_circle_parent_class)->destroy) CLUTTER_ACTOR_CLASS (iw_circle_parent_class)->destroy (self);
//}

/* use the actor's allocation for the ClutterBox */
static void iw_circle_allocate (ClutterActor *actor, const ClutterActorBox *box, ClutterAllocationFlags flags)
{
        //        IwCirclePrivate *priv = IW_CIRCLE (actor)->priv;
        //        ClutterActorBox child_box = {
        //                0,
        //        };

        //        /* set the allocation for the whole button */
        //        CLUTTER_ACTOR_CLASS (iw_circle_parent_class)->allocate (actor, box, flags);

        //        /* make the child (the ClutterBox) fill the parent;
        //         * note that this allocation box is relative to the
        //         * coordinates of the whole button actor, so we can't just
        //         * use the box passed into this function; instead, it
        //         * is adjusted to span the whole of the actor, from its
        //         * top-left corner (0,0) to its bottom-right corner
        //         * (width,height)
        //         */
        //        child_box.x1 = 0.0;
        //        child_box.y1 = 0.0;
        //        child_box.x2 = clutter_actor_box_get_width (box);
        //        child_box.y2 = clutter_actor_box_get_height (box);

        //        clutter_actor_allocate (priv->child, &child_box, flags);
}

/* paint function implementation: just calls paint() on the ClutterBox */
static void iw_circle_paint (ClutterActor *actor)
{
        IwCirclePrivate *priv = IW_CIRCLE (actor)->priv;

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

        //        //        cogl_set_source_color4ub (color.red, color.green, color.blue, tmp_alpha);
        //        cogl_set_source_color4ub (0, 0, 255, 128);

        //        /* create and store a path describing a star */
        //        cogl_path_move_to (width * 0.5, 0);
        //        cogl_path_line_to (width, height * 0.75);
        //        cogl_path_line_to (0, height * 0.75);
        //        cogl_path_move_to (width * 0.5, height);
        //        cogl_path_line_to (0, height * 0.25);
        //        cogl_path_line_to (width, height * 0.25);
        //        cogl_path_line_to (width * 0.5, height);

        cogl_path_ellipse (width / 2.0, height / 2.0, width / 2.0, height / 2.0);
        cogl_path_fill ();

        //        clutter_actor_paint (priv->child);
}

// static void iw_circle_paint_node (ClutterActor *actor, ClutterPaintNode *root)
//{
//        IwCirclePrivate *priv = IW_CIRCLE (actor)->priv;

//        ClutterPaintNode *node;
//        ClutterActorBox box;

//        // where the content of the actor should be painted
//        clutter_actor_get_allocation_box (actor, &box);

//        static const ClutterColor yellow_color = { 0x88, 0x88, 0x00, 0xff };
//        node = clutter_color_node_new (&yellow_color);
//        // the cogl_texture variable is set elsewhere
//        //        node = clutter_texture_node_new (cogl_texture, CLUTTER_COLOR_White, CLUTTER_SCALING_FILTER_TRILINEAR, CLUTTER_SCALING_FILTER_LINEAR);

//        // paint the content of the node using the allocation
//        clutter_paint_node_add_rectangle (node, &box);

//        // add the node, and transfer ownership
//        clutter_paint_node_add_child (root, node);
//        clutter_paint_node_unref (node);
//}

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

/* proxy ClickAction signals so they become signals from the actor */
static void iw_circle_clicked (ClutterClickAction *action, ClutterActor *actor, gpointer user_data)
{
        /* emit signal via the cache array */
        g_signal_emit (actor, iw_circle_signals[CLICKED], 0);
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
        gobject_class->set_property = iw_circle_set_property;
        gobject_class->get_property = iw_circle_get_property;

        // It still got destroyed even when I do not override the destroy method (like virtual function in C++).
        //        actor_class->allocate = iw_circle_allocate;
        //        actor_class->paint = iw_circle_paint;
        //        actor_class->paint_node = iw_circle_paint_node;
        actor_class->pick = star_actor_pick;

        g_type_class_add_private (klass, sizeof (IwCirclePrivate));

        /**
         * IwCircle:text:
         *
         * The text shown on the #IwCircle
         */
        pspec = g_param_spec_string ("text", "Text", "Text of the button", NULL, G_PARAM_READWRITE);
        g_object_class_install_property (gobject_class, PROP_TEXT, pspec);

        /**
         * IwCircle::clicked:
         * @button: the #IwCircle that emitted the signal
         *
         * The ::clicked signal is emitted when the internal #ClutterClickAction
         * associated with a #IwCircle emits its own ::clicked signal
         */
        iw_circle_signals[CLICKED] = g_signal_new ("clicked", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET (IwCircleClass, clicked), NULL,
                                                   NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
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
 * iw_circle_set_text:
 * @self: a #IwCircle
 * @text: the text to display on the button
 *
 * Set the text on the button
 */
void iw_circle_set_text (IwCircle *self, const gchar *text)
{
        //        IwCirclePrivate *priv;

        //        /* public API should check its arguments;
        //         * see also g_return_val_if_fail for functions which
        //         * return a value
        //         */
        //        g_return_if_fail (IW_IS_CIRCLE (self));

        //        priv = self->priv;

        //        g_free (priv->text);

        //        if (text)
        //                priv->text = g_strdup (text);
        //        else
        //                priv->text = g_strdup ("");

        //        /* call a function on the ClutterText inside the layout */
        //        clutter_text_set_text (CLUTTER_TEXT (priv->label), priv->text);
}

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

///**
// * iw_circle_set_text_color:
// * @self: a #IwCircle
// * @color: the #ClutterColor to use as the color for the button text
// *
// * Set the color of the text on the button
// */
// void iw_circle_set_text_color (IwCircle *self, const ClutterColor *color)
//{
//        g_return_if_fail (IW_IS_CIRCLE (self));

//        clutter_text_set_color (CLUTTER_TEXT (self->priv->label), color);
//}

///**
// * iw_circle_get_text:
// * @self: a #IwCircle
// *
// * Get the text displayed on the button
// *
// * Returns: the button's text. This must not be freed by the application.
// */
// const gchar *iw_circle_get_text (IwCircle *self)
//{
//        g_return_val_if_fail (IW_IS_CIRCLE (self), NULL);

//        return self->priv->text;
//}

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
