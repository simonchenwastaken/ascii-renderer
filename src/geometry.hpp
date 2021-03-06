#include <iostream>
#include <vector>
#include <math.h>

// Include Guard
#ifndef GEOMETRY
#define GEOMETRY

using namespace std;
using std::vector;

/**
 * @brief A vector in R3
 */
class Vector3
{
    public:
        double x;
        double y;
        double z;

        // Static vector operations

        /**
         * @brief Add v2 to v1
         */
        friend Vector3 operator+(const Vector3 v1, const Vector3 v2)
        {
            double x = v1.x + v2.x;
            double y = v1.y + v2.y;
            double z = v1.z + v2.z;

            return Vector3(x, y, z); 
        }

        /**
         * @brief Returns v1 scaled by a scalar
         */
        friend Vector3 operator*(const double scalar, const Vector3 v1)
        {
            double x = scalar * v1.x;
            double y = scalar * v1.y;
            double z = scalar * v1.z;

            return Vector3(x, y, z); 
        }

        /**
         * @brief Subtract v2 from v1
         */
        friend Vector3 operator-(const Vector3 v1, const Vector3 v2)
        {
            return v1 + (-1 * v2);
        }

        /**
         * @brief Return the dot product of two vectors
         */
        friend double operator*(const Vector3 v1, const Vector3 v2)
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        /**
        * @brief Return whether or not the vectors are equal
        */
        friend bool operator==(const Vector3 v1, const Vector3 v2)
        {
            static double epsilon = 0.005;
            Vector3 diff = v1 - v2;
            bool x = diff.x < epsilon && diff.x > -epsilon;
            bool y = diff.y < epsilon && diff.y > -epsilon;
            bool z = diff.z < epsilon && diff.z > -epsilon;

            return x && y && z;
        }

        /**
         * @brief Cast to string
         */
        operator std::string() const
        {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }

        /**
         * @brief Output to cout override
         */
        friend ostream & operator<<(ostream & out, const Vector3& v) 
        { 
            out << "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
            return out;
        }
 
        /**
         * @brief Return the cross product of two vectors
         */
        static Vector3 cross(Vector3 v1, Vector3 v2)
        {
            double x = v1.y * v2.z - v1.z * v2.y;
            double y = v1.z * v2.x - v1.x * v2.z;
            double z = v1.x * v2.y - v1.y * v2.x;

            return Vector3(x, y, z);
        }

        // Instance vector operations

        /**
         * @brief Returns the norm of the vector
         */
        double norm()
        {
            return sqrt(x * x + y * y + z * z);
        }

        /**
         * @brief Normalize the vector to a unit vector
         */
        void normalize()
        {
            double n = norm();

            if (n == 0)
        {
                return;
            }

            x = x / n;
            y = y / n;
            z = z / n;
        }

        // Constructors
        Vector3 ()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        Vector3(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
};


/**
 * @brief A line in R3
 */
class Line
{
    public:
        Vector3 d;
        Vector3 p;
    
        static Line getLine(Vector3 p1, Vector3 p2)
        {
            return Line(p2 - p1, p1);
        }

        Line(Vector3 d, Vector3 p)
        {
            this->d = d;
            this->p = p;
        }
};


/**
 * @brief A plane in R3
 */
class Plane
{
    public:
        Vector3 d1;
        Vector3 d2;
        Vector3 p;

        /**
         * @brief Return a tangent plane given a normal vector
         */
        static Plane getTangentPlane(Line line)
        {
            Vector3 a = Vector3(0.0f, 1.0f, 0.0f);
            // Edge case
            if (line.d.z == 0)
            {
                a = Vector3(0.0f, 0.0f, 1.0f);
            }
            else
            {
                // General case
                a.z = -line.d.y / line.d.z;
            }
            Vector3 b = Vector3::cross(a, line.d);
            
            return Plane(a, b, line.p);
        }

        /**
         * @brief Return a normal vector of the plane
         */
        Vector3 norm()
        {
            return Vector3::cross(d1, d2);
        }

        /**
         * @brief Return the intersection point between a line and a plane;
         */
        Vector3 lineIntersection(Line line)
        {
            Vector3 n = norm();
            double numerator = (n * line.p - n.x * line.p.x - n.y * line.p.y - n.z * line.p.z);
            double denominator = (n.x * line.d.x + n.y * line.d.y + n.z * line.d.z);

            // Edge case
            if (denominator == 0)
        {
                return Vector3();
            }

            // Solve for t
            double t = numerator / denominator;
            
            return line.p + t * line.d;
        }

        Plane()
        {
            d1 = Vector3(1.0f, 0.0f, 0.0f);
            d2 = Vector3(0.0f, 1.0f, 0.0f);
            p = Vector3(0.0f, 0.0f, 0.0f);
        }

        Plane(Vector3 d1, Vector3 d2, Vector3 p)
        {
            this->d1 = d1;
            this->d2 = d2;
            this->p = p;
        }
};

/**
 * @brief A triangular face in R3
 */
class Tri
{
    public:
        Vector3 p[3];
    
        Tri(Vector3 p1, Vector3 p2, Vector3 p3)
        {
            p[0] = p1;
            p[1] = p2;
            p[2] = p3;
        }

        /**
         * @brief Return the surface normal of the tri
         */
        Vector3 norm()
        {
            Vector3 d1 = p[1] - p[0];
            Vector3 d2 = p[2] - p[0];
            
            Vector3 norm = Vector3::cross(d1, d2);
            norm.normalize();
            return norm;
        }
};

/**
 * @brief A 3D object
 */
class Object
{
    public:
        Vector3 position;
        Vector3 rotation;
        vector<Tri> tris;

        Object()
        {
            
        }

        Object(vector<Tri> tris)
        {
            this->tris = tris;
        }
};

#endif /* GEOMETRY */