/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

/* inclusion guard */
#ifndef __IW_CIRCULAR_NODE_H__
#define __IW_CIRCULAR_NODE_H__

/* include any dependencies */
#include <clutter/clutter.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GObject implementation */

/* declare this function signature to remove compilation errors with -Wall;
 * the iw_circular_node_get_type() function is actually added via the
 * G_DEFINE_TYPE macro in the .c file
 */
GType iw_circular_node_get_type (void);

/* GObject type macros */
/* returns the class type identifier (GType) for IwCircularNode */
#define IW_TYPE_CIRCULAR_NODE (iw_circular_node_get_type ())

/* cast obj to a IwCircularNode object structure*/
#define IW_CIRCULAR_NODE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), IW_TYPE_CIRCULAR_NODE, IwCircularNode))

/* check whether obj is a IwCircularNode */
#define IW_IS_CIRCULAR_NODE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IW_TYPE_CIRCULAR_NODE))

/* cast klass to IwCircularNodeClass class structure */
#define IW_CIRCULAR_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), IW_TYPE_CIRCULAR_NODE, IwCircularNodeClass))

/* check whether klass is a member of the IwCircularNodeClass */
#define IW_IS_CIRCULAR_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IW_TYPE_CIRCULAR_NODE))

/* get the IwCircularNodeClass structure for a IwCircularNode obj */
#define IW_CIRCULAR_NODE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), IW_TYPE_CIRCULAR_NODE, IwCircularNodeClass))

/*
 * Private instance fields; see
 * http://www.gotw.ca/gotw/024.htm for the rationale
 */
typedef struct _IwCircularNodePrivate IwCircularNodePrivate;
typedef struct _IwCircularNode IwCircularNode;
typedef struct _IwCircularNodeClass IwCircularNodeClass;

/* object structure */
struct _IwCircularNode {
        /*<private>*/
        ClutterActor parent_instance;

        /* structure containing private members */
        /*<private>*/
        IwCircularNodePrivate *priv;
};

/* class structure */
struct _IwCircularNodeClass {
        /*<private>*/
        ClutterActorClass parent_class;
};

/* public API */

/* constructor - note this returns a ClutterActor instance */
ClutterActor *iw_circular_node_new (void);
void iw_circular_node_set_color (IwCircularNode *self, const ClutterColor *color);
void iw_circular_node_set_port_color (IwCircularNode *self, int i, const ClutterColor *color);
void iw_circular_node_set_port_angle (IwCircularNode *self, int i, float angle);
void iw_circular_node_set_port_size (IwCircularNode *self, int i, float s);
void iw_circular_node_set_radius (IwCircularNode *self, float r);
void iw_circular_node_set_ports_no (IwCircularNode *self, int i);

#ifdef __cplusplus
}
#endif

#endif /* __IW_CIRCUlar_nodE_H__ */
