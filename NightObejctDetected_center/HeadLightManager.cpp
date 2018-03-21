#include "HeadLightManager.h"

HeadLightManager::HeadLightManager()
{
}
HeadLightManager::~HeadLightManager()
{
}

void HeadLightManager::setLightObjects(vector<ObjectDetected> lightObjects)
{
	_lightObjects = lightObjects;
}

void HeadLightManager::setHeadLightPairs(Rect2d headLight, Mat& srcImg, int& id)
{
	ObjectTracker objectTracker;

	int iou = 0.8;
	int flag = -1;
	bool isRecover = false;

	/*if (_vectorOfObjectTracker.size() == 0)
	{
	id = 1;
	}*/

	for (int i = 0; i < _vectorOfObjectTracker.size(); i++)
	{
		Rect2d currentTrackPos = _vectorOfObjectTracker[i].getCurrentPos();
		//cout << "I = " << (currentTrackPos & headLight).area() << endl;
		//cout << "U = " << (currentTrackPos | headLight).area() << endl;

		//ckeck which object occupuies the most area
		if ((currentTrackPos & headLight).area() / (currentTrackPos | headLight).area() > iou)
		{
			//_vectorOfObjectTracker.erase(_vectorOfObjectTracker.begin() + i);
			flag = i;
			//iou = (currentTrackPos & headLight).area() / (currentTrackPos | headLight).area();
			break;
		}
	}

	//cout << "remove size: " << _vectorOfRemovedTracker.size() << endl;

	if (_vectorOfObjectTracker.size() > 0 && (flag != -1))
	{
		//cout << "update" << endl;
		int previousID = _vectorOfObjectTracker[flag].getID();
		//cout <<"prev" << previousID << endl;
		_vectorOfObjectTracker.erase(_vectorOfObjectTracker.begin() + flag);
		objectTracker.initialize(headLight, srcImg);
		objectTracker.setID(previousID);

	}
	else
	{
		for (int i = 0; i < _vectorOfRemovedTracker.size(); i++)
		{
			//cout << "removeID: " << _vectorOfRemovedTracker[i].getID() << endl;

			if (_vectorOfRemovedTracker[i].getCurrentPos().contains(Point(headLight.x, headLight.y)) ||
				abs(_vectorOfRemovedTracker[i].getCurrentPos().x - headLight.x) < 10)
			{
				objectTracker.initialize(headLight, srcImg);
				objectTracker.setID(_vectorOfRemovedTracker[i].getID());
				isRecover = true;
				break;
			}

		}
		//_vectorOfRemovedTracker.clear();

		if (!isRecover)
		{
			cout << id << endl;
			objectTracker.initialize(headLight, srcImg);
			objectTracker.setID(id);
		}

	}

	_vectorOfObjectTracker.push_back(objectTracker);

}

void HeadLightManager::updateHeadLightPairs(Mat& srcImg, Mat srcTemp)
{
	//check whether tracker is out of detect window or not
	for (int i = 0; i < _vectorOfObjectTracker.size(); i++)
	{
		Rect2d currentTrackPos = _vectorOfObjectTracker[i].getCurrentPos();

		/*stringstream ss;
		ss << "car " << i + 1;

		putText(srcImg, ss.str() , Point(currentTrackPos.x - 10, currentTrackPos.y - 10), 0, 1, Scalar(0, 0, 255), 3);*/

		if (currentTrackPos.x < _offsetX ||
			currentTrackPos.y < _offsetY ||
			currentTrackPos.x + currentTrackPos.width > _offsetX + _center.x + _center.width ||
			currentTrackPos.y + currentTrackPos.height > _offsetY + _center.y + _center.height)
		{
			_vectorOfObjectTracker.erase(_vectorOfObjectTracker.begin() + i);
		}
	}

	//check whether tracker is contain light object or not
	for (int i = 0; i < _vectorOfObjectTracker.size(); i++)
	{
		Rect2d currentTrackPos = _vectorOfObjectTracker[i].getCurrentPos();

		_vectorOfObjectTracker[i].clearObjectContain();

		for (int j = 0; j < _lightObjects.size(); j++)
		{
			if (currentTrackPos.contains(Point(_lightObjects[j].centroid.x, _lightObjects[j].centroid.y)))
			{
				_vectorOfObjectTracker[i].addObjectContain();
			}
		}
	}

	//cout << " Size: " << _vectorOfObjectTracker.size() << endl;

	for (int i = 0; i < _vectorOfObjectTracker.size(); i++)
	{
		if (_vectorOfObjectTracker[i].getNumberOfObjectContain() == 0)
		{
			_vectorOfRemovedTracker.push_back(_vectorOfObjectTracker[i]); // store removed if it can be detect later
			//cout << "Push removeID: " << _vectorOfRemovedTracker[i].getID() << endl;
			_vectorOfObjectTracker.erase(_vectorOfObjectTracker.begin() + i);
		}
	}

	//update
	for (int i = 0; i < _vectorOfObjectTracker.size(); i++)
	{
		_vectorOfObjectTracker[i].update(srcImg);
	}

	// draw the tracked object
	/*Rect2d extHeadLight;

	if (_headLight.x - 5 >= 0 && _headLight.y - 5 >= 0 && _headLight.width + 10 <= srcImg.cols && _headLight.height + 10 <= srcImg.rows)
	{
	extHeadLight.x = _headLight.x - 5;
	extHeadLight.y = _headLight.y - 5;
	extHeadLight.width = _headLight.width + 10;
	extHeadLight.height = _headLight.height + 10;
	rectangle(srcImg, extHeadLight, Scalar(255, 0, 0), 2, 1);
	}
	else
	{
	rectangle(srcImg, _headLight, Scalar(255, 0, 0), 2, 1);
	}*/
}

void HeadLightManager::setBackDetectRegion(vector<Rect> positions, int offsetX, int offsetY)
{
	_left = positions[0];
	_center = positions[1];
	_right = positions[2];
	_offsetX = offsetX;
	_offsetY = offsetY;
}

void HeadLightManager::setSideDetectRegion(vector<Rect> positions, int offsetX, int offsetY)
{
	_left = positions[0];
	_center = positions[1];
	_offsetX = offsetX;
	_offsetY = offsetY;
}

vector<ObjectTracker> HeadLightManager::getVectorOfObjectTracker()
{
	return _vectorOfObjectTracker;
}