/* Includes the Standard IO Library  */

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreturn-type"

int init_camera(){} /*initialize the camera before any other camera functions*/
int track_update(){} /*tracking data for a new frame and make it available for retrieval by the other vision functions*/
int track_get_frame(){} /*return value is the frame number used to generate the tracking data.*/
int track_count(int ch){} /*return the number of blobs available for the channel ch*/
int track_size(int ch, int i){} /* returns the number of pixels in the blob*/
int track_x(int ch, int i){} /*gets the pixel x coordinate of the centroid of the blob*/
int track_y(int ch, int i){}/*gets the pixel y coordinate of the centroid of the blob*/
int track_confidence(int ch, int i){} /*area/bounding box area (range 0-100, low numbers bad, high numbers good)*/
int track_bbox_left(int ch, int i){} /*gets the pixel x coordinate of the leftmost pixel in the blob i*/
int track_bbox_right(int ch, int i){} /*gets the pixel x coordinate of the rightmost pixel in the blob i*/
int track_bbox_top(int ch, int i){} /*gets the pixel y coordinate of the topmost pixel in the blob i*/
int track_bbox_bottom(int ch, int i){} /*gets the pixel y coordinate of the bottommost pixel in the blob i*/
int track_bbox_width(int ch, int i){} /*gets the pixel x width of the bounding box of the blob i*/
int track_bbox_height(int ch, int i){} /*gets the pixel y height of the bounding box of the blob i*/
float track_angle(int ch, int i){} /*gets the angle in radians of the major axis of the blob i*/
int track_major_axis(int ch, int i){} /*gets the length in pixels of the major  axis of the bounding ellipse*/
int track_minor_axis(int ch, int i){} /*gets the length in pixels of the minor axis of the bounding ellipse*/
void track_set_model_hsv(int ch, int h_min, int h_max, int s_min, int v_min){}
void track_get_model_hsv(int ch, int *h_min, int *h_max, int *s_min, int *v_min){}
// added 2/4/2010
int track_capture_time(){} /*Time since the last frame was captured*/
int mseconds(){} /* returns the time since boot */
