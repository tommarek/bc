/**
\file
 Bezier curves operations
*/
#include "bezier.h"

#include "framework/util.h"

/**
 * Create bezier curve
 * @param pointsCount bezier curve max control points count
 * @return bezier curve
 */
struct S_bezier *createBezier(GLuint pointsCount){
	struct S_bezier *result = checkMalloc(sizeof(struct S_bezier));

	result->controlPointsCount = 0;
	result->maxPointsCount = pointsCount;

	result->controlPoints = checkMalloc(sizeof(GLfloat) * 3 * pointsCount);
	result->helpPoints = checkMalloc(sizeof(GLfloat) * 3 * pointsCount);

	return result;
}

/**
 * Add control point to bezier curve
 * @param curve information about curve
 * @param x,y,z coordinates for placing new point
 */
void bezierAddControlPoint(struct S_bezier *curve, GLfloat x, GLfloat y, GLfloat z){
	// do adding is not possible when 
	if (curve->controlPointsCount >= curve->maxPointsCount) return;

	//add point
	curve->controlPoints[curve->controlPointsCount * 3 + 0] = x;
	curve->controlPoints[curve->controlPointsCount * 3 + 1] = y;
	curve->controlPoints[curve->controlPointsCount * 3 + 2] = z;


	// increment point count
	curve->controlPointsCount++;
}

/**
 * get actial position on curve.from real, start and stop time.
 * @param curve information about curve
 * @param realTime real time in demo
 * @param start start movement at
 * @param stop stop mevement at
 * @param result result point
 */
void bezierGetPointRealTimeLinear(struct S_bezier *curve, GLint realTime, GLint start, GLint stop, GLfloat *result){
	GLfloat t;

	if (realTime <= start){
		t = 0;
	}
	else if (realTime >= stop){
		t = 1;
	}
	// linear interpolation
	else{
		t = (realTime - start) / (GLfloat)(stop - start);
	}
	bezierGetPoint(curve, t, result);
}

/**
 * Get actual position according to time <0,1>. Using de Casteljau's algorithm.
 * @param curve information about curve
 * @param time parameter for de casteljau algorithm. must be between 0 and 1
 * @param result result point
 */
void bezierGetPoint(struct S_bezier *curve, GLfloat time, GLfloat *result){
	GLfloat p1[3], p2[3];

	// if no control points return 0
	if (curve->controlPointsCount == 0){
		myMemset(result, 0, sizeof(GLfloat) * 3);
		return;
	}
	// if one point return one point
	else if (curve->controlPointsCount == 1){
		for (int i = 0; i < 3; ++i){
			result[i] = curve->controlPoints[i];
		}
		return;
	}
	else{
		// copy controll points to temp points
		for(int i = 0; i < curve->controlPointsCount; ++i){
			for(int j = 0; j < 3; ++j){
				curve->helpPoints[(i * 3) + j] = curve->controlPoints[(i * 3) + j];
			}
		}
		// calculate actual point
		for (int i = curve->controlPointsCount; i > 1; --i){
			for (int j = 0; j < i; ++j){
				for (int k = 0; k < 3; ++k){
					p1[k] = curve->helpPoints[(j + 0) * 3 + k];
					p2[k] = curve->helpPoints[(j + 1) * 3 + k];
				}
				for (int k = 0; k < 3; ++k){
					curve->helpPoints[j * 3 + k] = Lerp(p1[k], p2[k], time);
				}
			}
		}
		// store point to result
		for (int i = 0; i < 3; ++i){
			result[i] = curve->helpPoints[i];
		}
	}
}


void drawCurve(struct S_bezier* curve){
	GLint i;
	GLfloat t;
	GLfloat v[3];

	glColor3f(0.5f,0.5f,1.0f);

	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 100; i++){
		t = (GLfloat)i / 100.0f;
		bezierGetPoint(curve, t, v);

		glVertex3f(v[0], v[1], v[2]);
	}
	glEnd();
	
}