#pragma once
#ifndef _VECTOR
#define _VECTOR
#include <vector>
#endif // !_VECTOR
#ifndef _TIME_H
#define _TIME_H
#include <time.h>
#endif // !_TIME_H

class block
{
	class Vec3
	{
	public:
		float ptr[3];
		Vec3(float v0, float v1, float v2)
		{
			ptr[0] = v0;
			ptr[1] = v1;
			ptr[2] = v2;
		}
		Vec3()
		{

		}
		inline float& operator[](size_t index)
		{
			return ptr[index];
		}
	};
	class Vec3i
	{
	public:
		int ptr[3];
		Vec3i(int v0, int v1, int v2)
		{
			ptr[0] = v0;
			ptr[1] = v1;
			ptr[2] = v2;
		}
		Vec3i()
		{

		}
		inline int& operator[](size_t index)
		{
			return ptr[index];
		}
	};
public:
	Vec3i pos;
	Vec3 color;
	std::vector<Vec3> blocks;
	std::vector<Vec3i> blocksi;
	size_t size;
	block() 
	{
		pos[0] = 5;
		pos[1] = 23;
		pos[2] = 5;
		size = 1;
		blocks.push_back(Vec3(0, 0, 0));
		blocksi.push_back(Vec3i(0, 0, 0));
	}
	block& operator=(const block& x)
	{
		this->pos = x.pos;
	}
	void add_block(int v0,int v1,int v2)
	{
		blocks.push_back(Vec3(v0 + 0.0001, v1 + 0.0001, v2 + 0.0001));
		blocksi.push_back(Vec3i(v0, v1, v2));
		size++;
	}
	void roll_left()
	{
		for (int v = 1; v < size; v++)
		{
			float flag0 = (blocks[v].ptr[0] * blocks[v].ptr[2] > 0 ? 1 : -1);
			float x0 = (blocks[v].ptr[0] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[2]);
			float x1 = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[0]);
			int x0i = (blocks[v].ptr[0] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[2]);
			int x1i = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[0]);
			blocks[v].ptr[0] = x0 * -flag0;
			blocks[v].ptr[2] = x1 * flag0;
			blocksi[v].ptr[0] = x0i*-flag0;
			blocksi[v].ptr[2] = x1i*flag0;
		}
	}
	void roll_right()
	{
		for (int v = 1; v < size; v++)
		{
			float flag0 = (blocks[v].ptr[0] * blocks[v].ptr[2] > 0 ? 1 : -1);
			float x0 = (blocks[v].ptr[0] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[2]);
			float x1 = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[0]);
			int x0i = (blocks[v].ptr[0] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[2]);
			int x1i = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[0]);
			blocks[v].ptr[0] = x0 * flag0;
			blocks[v].ptr[2] = x1 * -flag0;
			blocksi[v].ptr[0] = x0i * flag0;
			blocksi[v].ptr[2] = x1i * -flag0;
		}
	}
	void roll_up()
	{
		for (int v = 1; v < size; v++)
		{
			float flag0 = (blocks[v].ptr[1] * blocks[v].ptr[2] > 0 ? 1 : -1);
			float x0 = (blocks[v].ptr[1] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[2]);
			float x1 = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[1]);
			int x0i = (blocks[v].ptr[1] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[2]);
			int x1i = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[1]);
			blocks[v].ptr[1] = x0 * flag0;
			blocks[v].ptr[2] = x1 * -flag0;
			blocksi[v].ptr[1] = x0i * flag0;
			blocksi[v].ptr[2] = x1i * -flag0;
		}
	}
	void roll_down()
	{
		for (int v = 1; v < size; v++)
		{
			float flag0 = (blocks[v].ptr[1] * blocks[v].ptr[2] > 0 ? 1 : -1);
			float x0 = (blocks[v].ptr[1] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[2]);
			float x1 = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocks[v].ptr[1]);
			int x0i = (blocks[v].ptr[1] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[2]);
			int x1i = (blocks[v].ptr[2] > 0 ? 1 : -1)*std::abs(blocksi[v].ptr[1]);
			blocks[v].ptr[1] = x0 * -flag0;
			blocks[v].ptr[2] = x1 * flag0;
			blocksi[v].ptr[1] = x0i * -flag0;
			blocksi[v].ptr[2] = x1i * flag0;
		}
	}
};