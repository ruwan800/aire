/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "video.h"
#include "log.h"


#ifndef EDGE_H_
#define EDGE_H_

namespace aire {

class Edge {
public:
	Edge(Video vid);
	virtual ~Edge();
	std::vector<float> getEdgeVariation();
	std::vector<std::vector<float> > getSectionEdgeVariation();
private:
	Video video;
	Log* LOG;
	std::vector<float> edge_changes;
	std::vector<std::vector<float> > section_edge_changes;

};

} /* namespace aire */
#endif /* EDGE_H_ */
