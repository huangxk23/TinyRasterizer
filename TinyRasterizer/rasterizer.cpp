#include "rasterizer.h"

void rasterizer::clear()
{
	std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f(0, 0, 0));
	std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::max());
}

//drawing a line from x0 to x1 with x0 < x1 and slope between (0,1]
//flag determine whether transform back to slope [-1,0)
void rasterizer::draw_line_01(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1, bool flag)
{
	int x = x0.x(), y = x0.y();
	float d = (x0.y() - x1.y()) * (x + 1) + (x1.x() - x0.x()) * (y + 0.5) + x0.x() * x1.y() - x1.x() * x0.y();
	
	for (; x <= x1.x(); x++)
	{
		if (flag) set_pixel({ 2 * x0.x() - x, y }, color);
		else set_pixel({ x,y }, color);
		if (d < 0)
		{
			d += (x1.x() - x0.x()) + (x0.y() - x1.y());
			y += 1;
		}
		else
			d += (x0.y() - x1.y());
	}
}

//drawing a line from x0 to x1 with x0 < x1 and slope between (1,inf)
//flag determine whether transform back to slope (-inf,-1)
void rasterizer::draw_line_1inf(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1, bool flag)
{
	int x = x0.x(), y = x0.y();
	float d = (x0.y() - x1.y()) * (x + 0.5) + (x1.x() - x0.x()) * (y + 1) + x0.x() * x1.y() - x1.x() * x0.y();

	for (; y <= x1.y(); y++)
	{
		if (flag) set_pixel({ 2 * x0.x() - x,y }, color);
		else set_pixel({x,y},color);
		if (d < 0)
			d += (x1.x() - x0.x());
		else
		{
			x += 1;
			d += (x1.x() - x0.x()) + (x0.y() - x1.y());
		}
	}
}

//implementation of mid-point algorithom
void rasterizer::draw_line(const Eigen::Vector2i &start, const Eigen::Vector2i & end)
{
	if (start.x() == end.x())
	{
		for (int y = std::min(start.y(), end.y()); y <= std::max(start.y(), end.y()); y++)
			set_pixel({ start.x(),y }, color);
		return;
	}

	if (start.y() == end.y())
	{
		for (int x = std::min(start.x(), end.x()); x <= std::max(start.x(), end.x()); x++)
			set_pixel({ x,start.y() }, color);
		return;
	}
	
	float slope = static_cast<float>((end.y() - start.y())) / static_cast<float>((end.x() - start.x()));
	
	if (slope > 0 && slope <= 1)
	{
		if (start.x() < end.x() && start.y() < end.y()) draw_line_01(start, end, false);
		else draw_line_01(start, end, false);
		return;
	}
	else if (slope > 1)
	{
		if (start.x() < end.x() && start.y() < end.y()) draw_line_1inf(start, end, false);
		else draw_line_1inf(start, end, false);
		return;
	}
	else if (slope < -1)
	{
		if (end.x() < start.x() && end.y() > start.y())
		{
			Eigen::Vector2i x1(2 * start.x() - end.x(), end.y());
			draw_line_1inf(start, x1, true);
		}
		else
		{
			Eigen::Vector2i x1(2 * end.x() - start.x(), start.y());
			draw_line_1inf(end, x1, true);
		}
		return;
	}
	else
	{
		if (end.x() < start.x() && end.y() > start.y())
		{
			Eigen::Vector2i x1(2 * start.x() - end.x(), end.y());
			draw_line_01(start, x1, true);
		}
		else
		{
			Eigen::Vector2i x1(2 * end.x() - start.x(), start.y());
			draw_line_01(end, x1, true);
		}
		return;
	}

}


void rasterizer::set_pixel(const Eigen::Vector2i& coords, const Eigen::Vector3f& color)
{
	int idx = coords.y() * width + coords.x();
	frame_buf[idx] = color;
}



