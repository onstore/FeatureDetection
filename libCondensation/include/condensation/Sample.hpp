/*
 * Sample.hpp
 *
 *  Created on: 27.06.2012
 *      Author: poschmann
 */

#ifndef SAMPLE_HPP_
#define SAMPLE_HPP_

#include "opencv2/core/core.hpp"

using cv::Rect;

namespace condensation {

/**
 * Weighted sample representing a square image region with position and size (x, y, size) and according change (vx, vy, vsize).
 * The size is supposed to be the width of the sample, whereas the height depends on the aspect ratio of all samples. The change
 * of the size is not an offset, but a factor for the size, so 1 means no change.
 */
class Sample {
public:

	/**
	 * Constructs a new sample.
	 */
	Sample() : x(0), y(0), size(0), vx(0), vy(0), vsize(1), weight(1), object(false), clusterId(nextClusterId++) {}

	/**
	 * Constructs a new sample with velocities of zero and a weight of one.
	 *
	 * @param[in] x The x coordinate of the center.
	 * @param[in] y The y coordinate of the center.
	 * @param[in] size The size.
	 */
	Sample(int x, int y, int size) : x(x), y(y), size(size), vx(0), vy(0), vsize(1),  weight(1), object(false), clusterId(nextClusterId++) {}

	/**
	 * Constructs a new sample with a weight of one.
	 *
	 * @param[in] x The x coordinate of the center.
	 * @param[in] y The y coordinate of the center.
	 * @param[in] size The size.
	 * @param[in] vx The change of the x coordinate.
	 * @param[in] vy The change of the y coordinate.
	 * @param[in] vsize The change of the size (factor).
	 */
	Sample(int x, int y, int size, int vx, int vy, float vsize) :
			x(x), y(y), size(size), vx(vx), vy(vy), vsize(vsize),  weight(1), object(false), clusterId(nextClusterId++) {}

	~Sample() {}

	/**
	 * @return The square bounding box representing this sample.
	 */
	Rect getBounds() const {
		return Rect(x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight());
	}

	/**
	 * @return The x coordinate of the center.
	 */
	int getX() const {
		return x;
	}

	/**
	 * @param[in] x The new x coordinate of the center.
	 */
	void setX(int x) {
		this->x = x;
	}

	/**
	 * @return The y coordinate of the center.
	 */
	int getY() const {
		return y;
	}

	/**
	 * @param[in] y The new y coordinate of the center.
	 */
	void setY(int y) {
		this->y = y;
	}

	/**
	 * @return The size.
	 */
	int getSize() const {
		return size;
	}

	/**
	 * @param[in] size The new size.
	 */
	void setSize(int size) {
		this->size = size;
	}

	/**
	 * @return The width.
	 */
	int getWidth() const {
		return size;
	}

	/**
	 * @return The height.
	 */
	int getHeight() const {
		return cvRound(Sample::aspectRatio * size);
	}

	/**
	 * @return The change of the x coordinate.
	 */
	int getVx() const {
		return vx;
	}

	/**
	 * @param[in] x The new change of the x coordinate.
	 */
	void setVx(int vx) {
		this->vx = vx;
	}

	/**
	 * @return The change of the y coordinate.
	 */
	int getVy() const {
		return vy;
	}

	/**
	 * @param[in] y The new change of the y coordinate.
	 */
	void setVy(int vy) {
		this->vy = vy;
	}

	/**
	 * @return The change of the size (factor).
	 */
	float getVSize() const {
		return vsize;
	}

	/**
	 * @param[in] size The new change of the size (factor).
	 */
	void setVSize(float vsize) {
		this->vsize = vsize;
	}

	/**
	 * @return The weight.
	 */
	double getWeight() const {
		return weight;
	}

	/**
	 * Changes the weight.
	 *
	 * @param[in] weight The new weight.
	 */
	void setWeight(double weight) {
		this->weight = weight;
	}

	/**
	 * @return True if this sample represents the object, false otherwise.
	 */
	bool isObject() const {
		return object;
	}

	/**
	 * Changes whether this sample represents the object.
	 *
	 * @param[in] object Flag that indicates whether this sample represents the object.
	 */
	void setObject(bool object) {
		this->object = object;
	}

	/**
	 * @return The ID of the cluster this sample belongs to.
	 */
	int getClusterId() const {
		return clusterId;
	}

	/**
	 * Determines whether this sample is less than another sample using the weight. This sample is considered
	 * less than the other sample if the weight of this one is less than the weight of the other sample.
	 *
	 * @param[in] other The other sample.
	 * @return True if this sample comes before the other in a strict weak ordering, false otherwise.
	 */
	bool operator<(const Sample& other) const {
		return weight < other.weight;
	}

	/**
	 * Determines whether this sample is bigger than another sample using the weight. This sample is considered
	 * bigger than the other sample if the weight of this one is bigger than the weight of the other sample.
	 *
	 * @param[in] other The other sample.
	 * @return True if this sample comes before the other in a strict weak ordering, false otherwise.
	 */
	bool operator>(const Sample& other) const {
		return weight > other.weight;
	}

	/**
	 * Comparison function that compares samples by their weight in ascending order.
	 */
	class WeightComparisonAsc {
	public:
		bool operator()(const Sample& lhs, const Sample& rhs) {
			return lhs.weight < rhs.weight;
		}
	};

	/**
	 * Comparison function that compares samples by their weight in descending order.
	 */
	class WeightComparisonDesc {
	public:
		bool operator()(const Sample& lhs, const Sample& rhs) {
			return lhs.weight > rhs.weight;
		}
	};

	/**
	 * Changes the aspect ratio of all samples.
	 *
	 * @param[in] aspectRatio The new aspect ratio of all samples.
	 */
	static void setAspectRatio(double aspectRatio) {
		Sample::aspectRatio = aspectRatio;
	}

	/**
	 * Changes the aspect ratio of all samples to the ratio between the given width and height.
	 *
	 * @param[in] width The width to relate the given height to.
	 * @param[in] height The height defining the aspect ratio relative to the width.
	 */
	static void setAspectRatio(int width, int height) {
		setAspectRatio(static_cast<double>(height) / static_cast<double>(width));
	}

	static double aspectRatio; ///< The aspect ratio of all samples. Cannot be made private, because C++.
	static int nextClusterId;  ///< The next cluster ID that was not assigned to any sample before.

private:

	int x;         ///< The x coordinate of the center.
	int y;         ///< The y coordinate of the center.
	int size;      ///< The size.
	int vx;        ///< The change of the x coordinate.
	int vy;        ///< The change of the y coordinate.
	float vsize;   ///< The change of the size (factor).
	double weight; ///< The weight.
	bool object;   ///< Flag that indicates whether this sample represents the object.
	int clusterId; ///< ID of the cluster this sample belongs to.
};

} /* namespace condensation */
#endif /* SAMPLE_HPP_ */