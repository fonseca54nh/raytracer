#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

namespace rc
{
	template<typename T>
	class Vector3
	{
	protected:
		T x,y,z;
	public:
		Vector3(T n): x(n), y(n), z(n) {} 
		Vector3(T xx, T yy ,T zz) : x(xx), y(yy), z(zz) {};

		T getX() { return x; }
		T getY() { return y; }
		T getZ() { return z; }

		//inter vector operations
		Vector3<T> operator +(Vector3<T> v) { return Vector3(x+v.x, y+v.y, z+v.z); }	
		Vector3<T> operator -(Vector3<T> v) { return Vector3(x-v.x, y-v.y, z-v.z); }	
		Vector3<T> operator *(Vector3<T> v) { return Vector3(x*v.x, y*v.y, z*v.z); }	
		Vector3<T> operator /(Vector3<T> v) { return Vector3(x/v.x, y/v.y, z/v.z); }	
		

		//vector & constant operations
		Vector3<T> operator +(T number) { return Vector3(x+number, y+number, z+number); }
		Vector3<T> operator -(T number) { return Vector3(x-number, y-number, z-number); }
		Vector3<T> operator *(T number) { return Vector3(x*number, y*number, z*number); }
		Vector3<T> operator /(T number) { return Vector3(x/number, y/number, z/number); }

		//dot and cross product	
		T dot(Vector3<T> &v){ return x*v.x + y*v.y + z*v.z;  } 
		Vector3<T> cross(Vector3<T> &v) { return Vector3<T>( y*v.z, - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x ); }  
		
		//compute vector magnitude
		T magnitude() { return sqrt(x*x + y*y + z*z); }	
		
		//vector normalization
		Vector3<T> normalize() 
		{
			T inverse = 1/magnitude(); 
			x *= inverse, y*= inverse, z*= inverse;
			return *this;
		}

		void push( T xa, T ya, T za)
		{
				x = xa, y = ya, z = za;
		}
	};

	typedef rc::Vector3<float> vec3;
	typedef rc::Vector3<int> vec3i;
};

using namespace rc;


class ray
{
public:
	vec3 origin, direction;

	ray(vec3 a, vec3 b) : origin(a), direction(b) {};
};

class sphere 
{
public:
	vec3 center;
	float radius, transparency, t;
public:
	sphere(vec3 a, float b) : center(a), radius(b){};
	
	//compute intersection
	bool intersect(vec3 center, float r, float t, ray ar) 
	{
		//|| x - c|| = r*r
		//x = o + td => || o - c + td || = r*r


		vec3 v = ar.origin - center; 
		float b = 2 * v.dot(ar.direction);
		float c = v.dot(v) - r*r;

		//compute the delta
		float delta = b*b -4 * c;

		if(delta < 1e-4) return false;

		//compute the roots
		float t1 = ( - b + sqrt(delta) );
		float t2 = ( - b - sqrt(delta) );

		//assign the smallest root
		t = (t1 < t2) ? t1 : t2;

		return true;
	}

	float getR(){ return t; } 
	vec3 getNormal(vec3);
};

vec3 sphere::getNormal(vec3 pi) { return (pi - center) / radius; }

class light
{
protected:
	vec3 pos;
	vec3 color;
public:
	light(vec3 a, vec3 b) : pos(a), color(b)  {};
};

void clamp255(vec3& col) 
{

	float x = col.getX();
	float y = col.getY();
	float z = col.getZ();
	x = (x > 255) ? 255 : (x < 0) ? 0 : x;

	y = (y > 255) ? 255 : (y < 0) ? 0 : y;

	z = (z > 255) ? 255 : (z < 0) ? 0 : z;
	//col = (x,y,z);
	col.push(x,y,z);

}

int main()
{
	//image size
	int width = 500, height = 500;
	
	//colors definition
	vec3 white(255,255,255), blue(0,0,155), red(255,0,0), black(0,0,0);


	vec3 c(width * 0.5, height * 0.5, 50);
	//vec3 c(50,50,50);	
	float root;
	
	//create the spheres
	sphere a(c, 50);
	
	//light lg(pos, white);
	sphere b(vec3(0,0,50), 1);
	
	//cout << a.intersect(c, rad, root, ar); //efficient formats of debbuging

	//create the pixel buffer
	vec3 buffer(black);


	ofstream file("file.ppm");	
	file << "P3\n" << width << ' ' << height << ' ' << "255\n";

	for (int i = 0; i < height; i++) 
	{ 
		for (int j = 0; j < width; j++) 
		{
			buffer = black;

			ray ar(vec3(i,j,0),vec3(0,0,1));
			if (a.intersect(c, 50, root, ar)) 
		    {
				vec3 pi = ar.origin + (ar.direction*root);
				vec3 L = b.center - pi;
				vec3 N = a.getNormal(pi);
				L.normalize(), N.normalize();
				float dt = L.dot(N);
				buffer = (blue + white*dt) * 0.5;
				clamp255(buffer);
			}

		file << (int)buffer.getX()  << ' ' 
			 << (int)buffer.getY()  << ' ' 
			 << (int)buffer.getZ()  << '\n';
		}
	}
}
