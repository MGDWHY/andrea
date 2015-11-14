#include "LinearInterpolator.h"


namespace glutils {
	bool LinearInterpolator::FunctionPoint::operator<(FunctionPoint &other) {
		return this->T < other.T;
	};

	LinearInterpolator::FunctionPoint::FunctionPoint(float t, float value) {
		this->T = t;
		this->Value = value;
	}

	LinearInterpolator::FunctionPoint::FunctionPoint() {
		this->T = 1.0f;
		this->Value = 1.0f;
	}

	LinearInterpolator::LinearInterpolator(void){
		this->min = FunctionPoint();
		this->max = FunctionPoint();
		this->functionPoints = std::vector<FunctionPoint>();
	}


	LinearInterpolator::~LinearInterpolator(void){}


	void LinearInterpolator::AddFunctionPoint(float t, float value) {
		FunctionPoint point(t, value);
	
		this->functionPoints.push_back(point);
	
		std::sort(this->functionPoints.begin(), this->functionPoints.end());

		this->min = this->functionPoints.front();
		this->max = this->functionPoints.back();
	}

	float LinearInterpolator::GetValueAt(float t) {

		if(t <= this->min.T) {
			return this->min.Value;
		}

		if(t >= this->max.T) {
			return this->max.Value;
		}

		for(int i = 0; i < this->functionPoints.size() - 1; i++) {
			FunctionPoint * p1 = &(this->functionPoints[i]);
			FunctionPoint * p2 = &(this->functionPoints[i + 1]);

			if(t >= p1->T && t <= p2->T) {
				float relativeT = (t - p1->T) / (p2->T - p1->T);
				return p1->Value * (1.0f - relativeT) + p2->Value * relativeT;
			}
		}

		return 0.0f;
	}
}