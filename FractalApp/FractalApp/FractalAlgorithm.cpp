#include "stdafx.h"
#include "FractalAlgorithm.h"

void FractalAlgorithm::ShowColorPalette(Result * pt)
{
	pt->magnitude = pt->x_coordinate / (double)m_zoom.pixels;
}

FractalAlgorithm::FractalAlgorithm(Zoom & zoom, ColorPalette & palette):
	m_zoom(zoom),
	m_color(palette)
{
}

void FractalAlgorithm::CalculatePoint(Result * pt)
{
	
	switch (algoType) {
	case AlgorithmType::ShowColorPalette: ShowColorPalette(pt);
	}

}
