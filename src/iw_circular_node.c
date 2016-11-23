/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <math.h>
#include "iw_circular_node.h"

/* convenience macro for GType implementations; see:
 * http://library.gnome.org/devel/gobject/2.27/gobject-Type-Information.html#G-DEFINE-TYPE:CAPS
 */
G_DEFINE_TYPE (IwCircularNode, iw_circular_node, CLUTTER_TYPE_ACTOR);

/* macro for accessing the object's private structure */
#define IW_CIRCULAR_NODE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), IW_TYPE_CIRCULAR_NODE, IwCircularNodePrivate))

/**
 * One input.
 */
struct _IwCircularNodePort {
        float angle;
        float size;
        ClutterColor color;
};

typedef struct _IwCircularNodePort IwCircularNodePort;

#define MAX_PORTS_NO 16

/*
 * Private structures.
 */
struct _IwCircularNodePrivate {
        ClutterColor color;
        ClutterContent *canvas;
        float radius;

        int portsNo;
        IwCircularNodePort ports[MAX_PORTS_NO];
};

static gboolean do_draw (ClutterCanvas *canvas, cairo_t *cr, int width, int height, gpointer *data);
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
static void iw_circular_node_finalize (GObject *gobject)
{
        //        IwCircularNodePrivate *priv = IW_CIRCULAR_NODE (gobject)->priv;
        //        clutter_color_free (priv->color);

        /* call the parent class' finalize() method */
        G_OBJECT_CLASS (iw_circular_node_parent_class)->finalize (gobject);
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
static void iw_circular_node_class_init (IwCircularNodeClass *klass)
{
        ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
        GParamSpec *pspec;

        gobject_class->finalize = iw_circular_node_finalize;
        //        gobject_class->set_property = iw_circular_node_set_property;
        //        gobject_class->get_property = iw_circular_node_get_property;

        // It still got destroyed even when I do not override the destroy method (like virtual function in C++).
        //        actor_class->allocate = iw_circular_node_allocate;
        //        actor_class->paint = iw_circular_node_paint;
        //        actor_class->paint_node = iw_circular_node_paint_node;
        actor_class->pick = star_actor_pick;

        g_type_class_add_private (klass, sizeof (IwCircularNodePrivate));
}

/* object init: create a private structure and pack
 * composed ClutterActors into it
 */
static void iw_circular_node_init (IwCircularNode *self)
{
        IwCircularNodePrivate *priv;
        ClutterLayoutManager *layout;

        priv = self->priv = IW_CIRCULAR_NODE_GET_PRIVATE (self);
        priv->portsNo = 0;

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

        //        g_signal_connect (priv->click_action, "clicked", G_CALLBACK (iw_circular_node_clicked), NULL);
        priv->color = *clutter_color_get_static (CLUTTER_COLOR_WHITE);
        priv->canvas = clutter_canvas_new ();
        //        clutter_canvas_set_size (CLUTTER_CANVAS (priv->canvas), 300, 300);
        clutter_actor_set_content (CLUTTER_ACTOR (self), priv->canvas);
        clutter_actor_set_content_scaling_filters (CLUTTER_ACTOR (self), CLUTTER_SCALING_FILTER_TRILINEAR, CLUTTER_SCALING_FILTER_LINEAR);
        g_object_unref (priv->canvas);

        /* connect our drawing code */
        g_signal_connect (priv->canvas, "draw", G_CALLBACK (do_draw), priv);
        /* invalidate the canvas, so that we can draw before the main loop starts */
        clutter_content_invalidate (priv->canvas);

        g_signal_connect (CLUTTER_ACTOR (self), "allocation-changed", G_CALLBACK (on_actor_resize), NULL);
}

/*****************************************************************************/

static gboolean do_draw (ClutterCanvas *canvas, cairo_t *cr, int width, int height, gpointer *data)
{
        IwCircularNodePrivate *priv = (IwCircularNodePrivate *)data;

        cairo_save (cr);

        /* clear the contents of the canvas, to avoid painting
         * over the previous frame
         */
        cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
        cairo_paint (cr);
        cairo_restore (cr);
        cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

        cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_width (cr, 0);

#define PORT_LINE_WIDTH 5

        /* the black rail that holds the seconds indicator */
        clutter_cairo_set_source_color (cr, &priv->color);
        cairo_translate (cr, priv->radius, priv->radius);
        cairo_arc (cr, 0, 0, priv->radius - PORT_LINE_WIDTH - PORT_LINE_WIDTH / 2 - 1, 0, G_PI * 2);
        cairo_fill (cr);

        cairo_set_line_width (cr, PORT_LINE_WIDTH);
        for (int i = 0; i < priv->portsNo; ++i) {
                clutter_cairo_set_source_color (cr, &priv->ports[i].color);
                cairo_arc (cr, 0, 0, priv->radius - PORT_LINE_WIDTH / 2 - 1, priv->ports[i].angle - priv->ports[i].size / 2,
                           priv->ports[i].angle + priv->ports[i].size / 2);
                cairo_stroke (cr);
        }

        /* we're done drawing */
        return TRUE;
}

// static guint idle_resize_id;

// static gboolean idle_resize (gpointer data)
//{
//        ClutterActor *actor = data;
//        float width, height;

//        /* match the canvas size to the actor's */
//        clutter_actor_get_size (actor, &width, &height);
//        clutter_canvas_set_size (CLUTTER_CANVAS (clutter_actor_get_content (actor)), ceilf (width), ceilf (height));

//        /* unset the guard */
//        idle_resize_id = 0;

//        /* remove the timeout */
//        return G_SOURCE_REMOVE;
//}

// static void on_actor_resize (ClutterActor *actor, const ClutterActorBox *allocation, ClutterAllocationFlags flags, gpointer user_data)
//{
//        /* throttle multiple actor allocations to one canvas resize; we use a guard
//         * variable to avoid queueing multiple resize operations
//         */
//        if (idle_resize_id == 0) {
//                idle_resize_id = clutter_threads_add_timeout (1000, idle_resize, actor);
//        }
//}

static void on_actor_resize (ClutterActor *actor, const ClutterActorBox *allocation, ClutterAllocationFlags flags, gpointer user_data)
{
        clutter_canvas_set_size (CLUTTER_CANVAS (clutter_actor_get_content (actor)), ceilf (clutter_actor_box_get_width (allocation)),
                                 ceilf (clutter_actor_box_get_height (allocation)));
}

/* public API */
/* examples of public API functions which wrap functions
 * on internal actors
 */

/*****************************************************************************/

void iw_circular_node_set_color (IwCircularNode *self, const ClutterColor *color)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        self->priv->color = *color;
        clutter_content_invalidate (self->priv->canvas);
}

/*****************************************************************************/

void iw_circular_node_set_ports_no (IwCircularNode *self, int i)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        self->priv->portsNo = i;
}

/*****************************************************************************/

void iw_circular_node_set_port_color (IwCircularNode *self, int i, const ClutterColor *color)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        self->priv->ports[i].color = *color;
        clutter_content_invalidate (self->priv->canvas);
}

/*****************************************************************************/

void iw_circular_node_set_port_angle (IwCircularNode *self, int i, float angle)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        self->priv->ports[i].angle = angle;
        clutter_content_invalidate (self->priv->canvas);
}

/*****************************************************************************/

void iw_circular_node_set_port_size (IwCircularNode *self, int i, float s)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        g_return_if_fail (i >= 0);
        g_return_if_fail (i < self->priv->portsNo);
        self->priv->ports[i].size = s;
        clutter_content_invalidate (self->priv->canvas);
}

/*****************************************************************************/

ClutterActor *iw_circular_node_new (void) { return g_object_new (IW_TYPE_CIRCULAR_NODE, NULL); }

/*****************************************************************************/

void iw_circular_node_set_radius (IwCircularNode *self, float r)
{
        g_return_if_fail (IW_IS_CIRCULAR_NODE (self));
        self->priv->radius = r;
        clutter_actor_set_size (CLUTTER_ACTOR (self), r * 2, r * 2);
        clutter_content_invalidate (self->priv->canvas);
}
