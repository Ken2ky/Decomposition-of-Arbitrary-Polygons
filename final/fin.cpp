#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
typedef std::vector<int> IntList;
typedef std::vector<std::pair<int, int>> PairList;
typedef std::vector<bool> BoolList;
 
set<int> bablu;

/// @brief The vertex class is used for storing the vertex number and its coordinates.
///
/// It allows us to assign the previous and next vertices to a vertex

class Vertex
{
public:
    int id;
    double x;
    double y;
    Vertex *prev;
    Vertex *next;
    Vertex(int id, double x, double y) : id(id), x(x), y(y), prev(nullptr), next(nullptr) {}
};

/// @brief The halfedge class represents a directed edge between vertices
///
///It also stores information regarding the previous and next edges
class HalfEdge
{
public:
    Vertex *start;
    Vertex *end;
    HalfEdge *twin;
    HalfEdge *prev;
    HalfEdge *next;
    HalfEdge(Vertex *start, Vertex *end) : start(start), end(end), twin(nullptr), prev(nullptr), next(nullptr) {}
};
 
/// @brief The Face class is used for finding the direction for navigating
class Face
{
public:
    HalfEdge *edge;
    Face(HalfEdge *edge) : edge(edge) {}
};
 

/// @brief The DCEL class stores all the vertices, edges and faces
class DCEL
{
public:
    vector<Vertex *> vertices;
    vector<HalfEdge *> halfEdges;
    vector<Face *> faces;
 
    Vertex *addVertex(int id, double x, double y)
    {
        Vertex *vertex = new Vertex(id, x, y);
        vertices.push_back(vertex);
        return vertex;
    }
 
    HalfEdge *addHalfEdge(Vertex *start, Vertex *end)
    {
        HalfEdge *halfEdge = new HalfEdge(start, end);
        halfEdges.push_back(halfEdge);
        return halfEdge;
    }
 
    Face *addFace(HalfEdge *edge)
    {
        Face *face = new Face(edge);
        faces.push_back(face);
        return face;
    }
};
 
/** @file */

/// @brief This function is used to find the angle when given three vertices(inner angle)
/// @param V1 Vertex 1
/// @param V2 Vertex 2
/// @param V3 Vertex 3
/// @return Angle made by 123
double findAngle(Vertex *V1, Vertex *V2, Vertex *V3)
{
    // calculate the vectors v1v2 and v2v3
    double v1v2_x = V2->x - V1->x;
    double v1v2_y = V2->y - V1->y;
    double v2v3_x = V3->x - V2->x;
    double v2v3_y = V3->y - V2->y;
 
    // check for NaN values
    if (std::isnan(v1v2_x) || std::isnan(v1v2_y) || std::isnan(v2v3_x) || std::isnan(v2v3_y))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
 
    // calculate the dot product of v1v2 and v2v3
    double dotProduct = v1v2_x * v2v3_x + v1v2_y * v2v3_y;
 
    // calculate the magnitudes of v1v2 and v2v3
    double v1v2_mag = sqrt(v1v2_x * v1v2_x + v1v2_y * v1v2_y);
    double v2v3_mag = sqrt(v2v3_x * v2v3_x + v2v3_y * v2v3_y);
 
    // check for NaN values
    if (std::isnan(v1v2_mag) || std::isnan(v2v3_mag) || std::isnan(dotProduct))
    {
        cout << "mag is nan" << endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
 
    // calculate the cosine of the angle between v1v2 and v2v3
    double cosine = dotProduct / (v1v2_mag * v2v3_mag);
    // check for NaN values
    if (std::isnan(cosine))
    {
        cout << "cosine is nan" << endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
 
    // calculate the angle in radians
    double angle_rad;
    if (cosine < 1 && cosine > -1)
        angle_rad = acos(cosine);
    else if (cosine >= 1)
        angle_rad = 0;
    else
        angle_rad = M_PI;
 
    // check for NaN values
    if (std::isnan(angle_rad))
    {
        cout << "angle is nan" << endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
 
    // convert the angle to degrees
    double angle_deg = angle_rad * 180 / M_PI;
 
    // determine the sign of the angle based on the cross product of v1v2 and v2v3
    double crossProduct = v1v2_x * v2v3_y - v1v2_y * v2v3_x;
 
    // check for NaN values
    if (std::isnan(crossProduct))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
 
    if (crossProduct > 0)
    {
        angle_deg = 360 - angle_deg;
    }
 
    return angle_deg;
}
/// @brief Checks if two edges are equal, which means they share the same set of vertices(the order of vertices doesn't matter)
/// @param e1 Edge 1
/// @param e2 Edge 2
/// @return True or False
bool isEqualEdge(HalfEdge *e1, HalfEdge *e2) // check if two edges are equal, which means they share the same set of vertices(the order of vertices doesn't matter)
{
    if (e1->start->id == e2->start->id && e1->end->id == e2->end->id)
    {
        return true;
    }
    else if (e1->start->id == e2->end->id && e1->end->id == e2->start->id)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// @brief Finds the maximum x-coordinate of the given set stored in the vector
/// @param s Set containing indices 
/// @param P Vector with vertices
/// @return Maximum value of x-coordinate
double findMaxX(set<int> &s, vector<Vertex *> &P)
{
    double max = 0;
    for (auto it = s.begin(); it != s.end(); it++)
    {
        if (P[*it]->x > max)
        {
            max = P[*it]->x;
        }
    }
    return max;
}
/// @brief Finds the maximum y-coordinate of the given set stored in the vector
/// @param s Set containing indices 
/// @param P Vector with vertices
/// @return maximum value of y-coordinate
double findMaxY(set<int> &s, vector<Vertex *> &P)
{
    double max = 0;
    for (auto it = s.begin(); it != s.end(); it++)
    {
        if (P[*it]->y > max)
        {
            max = P[*it]->y;
        }
    }
    return max;
}
/// @brief Finds the minimum x-coordinate of the given set stored in the vector
/// @param s Set containing indices 
/// @param P Vector with vertices
/// @return minimum value of x-coordinate

double findMinX(set<int> &s, vector<Vertex *> &P)
{
    double min = INT_MAX;
    for (auto it = s.begin(); it != s.end(); it++)
    {
        if (P[*it]->x < min)
        {
            min = P[*it]->x;
        }
    }
    return min;
}

/// @brief Finds the minimum y-coordinate of the given set stored in the vector
/// @param s Set containing indices 
/// @param P Vector with vertices
/// @return minimum value of y-coordinate

double findMinY(set<int> &s, vector<Vertex *> &P)
{
    double min = INT_MAX;
    for (auto it = s.begin(); it != s.end(); it++)
    {
        if (P[*it]->y < min)
        {
            min = P[*it]->y;
        }
    }
    return min;
}
/// @brief Check if p3 and p4 lie on the same side of the line formed by p1 and p2
/// @param p1 Vertex 1
/// @param p2 Vertex 2
/// @param p3 Vertex 3
/// @param p4 Vertex 4
/// @return True or False
bool isSameHalfPlane(Vertex *p1, Vertex *p2, Vertex *p3, Vertex *p4)
{
    // check if p3 and p4 lie on the same side of the line formed by p1 and p2
    double x1 = p1->x;
    double y1 = p1->y;
    double x2 = p2->x;
    double y2 = p2->y;
    double x3 = p3->x;
    double y3 = p3->y;
    double x4 = p4->x;
    double y4 = p4->y;
    // check if p3 and p4 lie on the same side of the line formed by p1 and p2
    double val1 = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    double val2 = (x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1);
    if (val1 * val2 > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// @brief Prints all the dcels stored
/// @param dcel DCEL object
void printDCEL(DCEL *dcel)
{
    // cout << "Vertices:" << endl;
    // for (auto vertex : dcel->vertices)
    // {
    //     cout << "id: " << vertex->id << ", x: " << vertex->x << ", y: " << vertex->y << endl;
    //     // print prev and next IDs
    //     cout << "prev: ";
    //     if (vertex->prev != NULL)
    //     {
    //         cout << vertex->prev->id << endl;
    //     }
    //     else
    //     {
    //         cout << "NULL" << endl;
    //     }
    //     cout << "next: ";
    //     if (vertex->next != NULL)
    //     {
    //         cout << vertex->next->id << endl;
    //     }
    //     else
    //     {
    //         cout << "NULL" << endl;
    //     }
    // }
    // cout << endl;
 
    cout << "[";
    for (auto halfEdge : dcel->halfEdges)
    {
        cout << "(" << halfEdge->start->x << "," << halfEdge->start->y << "),(" << halfEdge->end->x << "," << halfEdge->end->y << "),";
    }
    cout << "]" << endl;
 
    // cout << "Faces:" << endl;
    // for (auto face : dcel->faces)
    // {
    //     cout << "edges: ";
    //     HalfEdge *edge = face->edge;
    //     do
    //     {
    //         cout << "(" << edge->start->x << ", " << edge->start->y << ") -> (" << edge->end->x << ", " << edge->end->y << ") ";
    //         edge = edge->next;
    //     } while (edge != face->edge);
    //     cout << endl;
    // }
    // cout << endl;
}

/// @brief This function is used for removing unneccessay diagonals i.e. combine convex ploygons(if the removal of diagonals leads to formation of convex polygon)
/// @param dcel1 DCEL 1
/// @param dcel2 DCEL 2 
/// @param diagStart Vertex 1 of the diagonal
/// @param diagEnd Vertex 2 of the diagonal
/// @return 
DCEL mergeDCELs(DCEL dcel1, DCEL dcel2, Vertex *diagStart, Vertex *diagEnd)
{
    // Merge vertices
    vector<Vertex *> mergedVertices = dcel1.vertices;
    for (Vertex *v : dcel2.vertices)
    {
        bool found = false;
        for (Vertex *mv : mergedVertices)
        {
            if (v->id == mv->id)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            mergedVertices.push_back(v);
        }
    }
    // print number of merged vertices
    cout << "Merged vertices: " << mergedVertices.size() << endl;
    // Merge half-edges (Remove duplicates and diagonals)
    vector<HalfEdge *> mergedHalfEdges = dcel1.halfEdges;
    for (HalfEdge *he : dcel2.halfEdges)
    {
        bool found = false;
        for (HalfEdge *mhe : mergedHalfEdges)
        {
            if (he->start->id == mhe->start->id && he->end->id == mhe->end->id)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            mergedHalfEdges.push_back(he);
        }
    }
    // Remove diagonals
    for (HalfEdge *he : mergedHalfEdges)
    {
        if (he->start->x == diagStart->x && he->start->y == diagStart->y && he->end->x == diagEnd->x && he->end->y == diagEnd->y)
        {
            cout << "Removing diagonal: (" << he->start->x << ", " << he->start->y << ") -> (" << he->end->x << ", " << he->end->y << ")" << endl;
            mergedHalfEdges.erase(remove(mergedHalfEdges.begin(), mergedHalfEdges.end(), he), mergedHalfEdges.end());
        }
        if (he->end->x == diagStart->x && he->end->y == diagStart->y && he->start->x == diagEnd->x && he->start->y == diagEnd->y)
        {
            cout << "Removing diagonal: (" << he->start->x << ", " << he->start->y << ") -> (" << he->end->x << ", " << he->end->y << ")" << endl;
            mergedHalfEdges.erase(remove(mergedHalfEdges.begin(), mergedHalfEdges.end(), he), mergedHalfEdges.end());
        }
    }
    // Update the ordering of half-edges vector such that end vertex of each half-edge is the start vertex of the next half-edge
    for (int i = 0; i < mergedHalfEdges.size(); i++)
    {
        for (int j = i + 1; j < mergedHalfEdges.size(); j++)
        {
            if (mergedHalfEdges[i]->end->id == mergedHalfEdges[j]->start->id)
            {
                swap(mergedHalfEdges[i + 1], mergedHalfEdges[j]);
            }
        }
    }
 
    // Create new DCEL object
    DCEL mergedDCEL;
    mergedDCEL.vertices = mergedVertices;
    mergedDCEL.halfEdges = mergedHalfEdges;
 
    return mergedDCEL;
}
 
int main()
{
    auto start = high_resolution_clock::now();
    int m = 0;
    int noOfVerticesInOriginalPolygon = 0;
    vector<DCEL> dcel(100);
    DCEL copyOfOriginalPolygon;
    int n;
    cout << "Enter the number of vertices" << endl;
    cin >> n;
    int siz = n;
    vector<Vertex *> P(n + 1);
    vector<Vertex *> P1(n + 1);
    Vertex *v1 = new Vertex(0, 0, 0);
    P[0] = v1;
    P1[0] = v1;
    for (int i = 1; i < n + 1; i++)
    {
        double a, b;
        cin >> a >> b;
        Vertex *v1 = new Vertex(i, a, b);
        P[i] = v1;
        P1[i] = v1;
        noOfVerticesInOriginalPolygon++;
    }
 
    set<int> L;
    set<int> N;
    n = siz;
    while (n > 3)
    {
        L.insert(1);
        L.insert(2);
        int k = 2;
        while (L.size() < n)
        {
            if (findAngle(P[k - 1], P[k], P[k + 1]) <= 180 && findAngle(P[k], P[k + 1], P[1]) <= 180 && findAngle(P[k + 1], P[1], P[2]) <= 180)
            {
                L.insert(k + 1);
                k++;
            }
            else
            {
                break;
            }
        }
        if (L.size() != n)
        {
 
            set<int> S;
            S.clear();
            for (int i = 1; i < siz + 1; i++)
            {
                for (int j = 1; j < n + 1; j++)
                {
                    if (L.count(j) && P1[i] == P[j])
                    {
                        S.insert(i);
                        break;
                    }
                }
            }
            if (m != 0)
            {
                for (int i = 1; i < siz + 1; i++)
                {
                    if (!S.count(i))
                    {
                        N.insert(i);
                    }
                }
            }
            else
            {
                for (int i = 1; i < siz + 1; i++)
                {
                    if (!L.count(i))
                    {
                        N.insert(i);
                    }
                }
            }
        }
 
        int min_x, max_x, min_y, max_y;
        while (N.size() > 0)
        {
 
            min_x = findMinX(L, P);
            max_x = findMaxX(L, P);
            min_y = findMinY(L, P);
            max_y = findMaxY(L, P);
 
            bool b_ward = false;
 
            while (!b_ward && N.size() > 0)
            {
                auto it = N.begin();
                if (P1[*it]->x <= min_x || P1[*it]->x >= max_x || P1[*it]->y <= min_y || P1[*it]->y >= max_y)
                {
                    N.erase(*it);
                    it = N.begin();
                }
                else
                {
                    if (N.size() > 0)
                    {
                        auto t = L.end();
                        t--;
                        while (1)
                        {
                            if (isSameHalfPlane(P[1], P[*t], P[*(t)-1], P1[*it]))
                            {
                                L.erase(*t);
                                t = L.end();
                                t--;
                            }
                            else if (isSameHalfPlane(P[2], P[1], P[*t], P1[*it]))
                            {
                                L.erase(*t);
                                t = L.end();
                                t--;
                            }
                            else
                            {
                                break;
                            }
                        }
                        N.erase(*it);
                    }
 
                    b_ward = true;
                }
            }
        }
 
        auto l = (--L.end());
        if (*l != 2)
        {
            vector<HalfEdge *> ed(L.size() - 1);
            int num = 0;
            for (auto it = L.begin(); it != (--L.end()); it++)
            {
                HalfEdge *e1 = dcel[m + 1].addHalfEdge(P[*it], P[*(it) + 1]);
                ed[num] = e1;
                num++;
            }
 
            HalfEdge *e1 = dcel[m + 1].addHalfEdge(P[*l], P[1]);
            int nin = 1;
            int j = 0;
            for (int i = 0; i < n; i++)
            {
                if (L.count(i + 1))
                {
                    Vertex *v3 = dcel[m + 1].addVertex(P[i + 1]->id, P[i + 1]->x, P[i + 1]->y);
                    nin++;
                }
            }
            for (int i = 0; i < dcel[m + 1].vertices.size() - 1; i++)
            {
                dcel[m + 1].vertices[i]->next = dcel[m + 1].vertices[i + 1];
                dcel[m + 1].vertices[i + 1]->prev = dcel[m + 1].vertices[i];
            }
            dcel[m + 1].vertices[dcel[m + 1].vertices.size() - 1]->next = dcel[m + 1].vertices[0];
            dcel[m + 1].vertices[0]->prev = dcel[m + 1].vertices[dcel[m + 1].vertices.size() - 1];
            P[0] = P[1];
 
            P[1] = P[*l];
 
            for (int i = 0; i < n; i++)
            {
                if (!L.count(i + 1))
                {
                    P[j + 2] = P[i + 1];
                    j++;
                }
            }
 
            P[j + 2] = P[0];
            P[0] = P1[0];
            n = n - L.size() + 2;
            if (n == 3)
            {
                dcel[m + 2].addVertex(P[1]->id, P[1]->x, P[1]->y);
                int mo = dcel[m + 1].vertices[dcel[m + 1].vertices.size() - 1]->id;
                dcel[m + 2].addVertex(P[2]->id, P[2]->x, P[2]->y);
                dcel[m + 2].addVertex(P[3]->id, P[3]->x, P[3]->y);
                dcel[m + 2].addHalfEdge(P[1], P[2]);
                dcel[m + 2].addHalfEdge(P[2], P[3]);
                dcel[m + 2].addHalfEdge(P[3], P[1]);
                dcel[m + 2].vertices[0]->next = dcel[m + 2].vertices[1];
                dcel[m + 2].vertices[1]->prev = dcel[m + 2].vertices[0];
                dcel[m + 2].vertices[1]->next = dcel[m + 2].vertices[2];
                dcel[m + 2].vertices[2]->prev = dcel[m + 2].vertices[1];
                dcel[m + 2].vertices[2]->next = dcel[m + 2].vertices[0];
                dcel[m + 2].vertices[0]->prev = dcel[m + 2].vertices[2];
            }
            L.clear();
            N.clear();
            m++;
        }
        else
        {
            P[0] = P[1];
            for (int i = 1; i < n; i++)
            {
                P[i] = P[i + 1];
            }
            P[n] = P[0];
            P[0] = P1[0];
            L.clear();
            N.clear();
        }
    }
    // Merge starts here
    vector<tuple<HalfEdge *, int, int>> LLE; // stores all the diagonals between the decomposition and the polygons to the left and right of the diagonal
    // fill LLE
    for (int i = 0; i < m + 2; i++)
    {
        // find the diagonals between the decompositions (i and all other j decompostions and add to LLE)
        for (int j = i + 1; j < m + 2; j++)
        {
            // find common diagonals using the isEqualsEdge fucntion
            for (auto it = dcel[i].halfEdges.begin(); it != dcel[i].halfEdges.end(); it++)
            {
                for (auto it1 = dcel[j].halfEdges.begin(); it1 != dcel[j].halfEdges.end(); it1++)
                {
                    if (isEqualEdge(*it, *it1))
                    {
                        LLE.push_back(make_tuple(*it, i, j));
                    }
                }
            }
        }
    }
    int verticesPerDecomposition[m + 2];
    // find the number of vertices in each decomposition
    for (int i = 0; i < m + 2; i++)
    {
        verticesPerDecomposition[i] = dcel[i].vertices.size();
    }
    // print the number of vertices in each decomposition
    // for (int i = 0; i < m + 2; i++)
    // {
    //     cout << "Number of vertices in decomposition " << i + 1 << " is " << verticesPerDecomposition[i] << endl;
    // }
    // print LLE
    // cout << "Diagonals: " << endl;
    // for (auto it = LLE.begin(); it != LLE.end(); it++)
    // {
    //     cout << "Diagonal: " << (*get<0>(*it)).start->id << " " << (*get<0>(*it)).end->id << " " << get<1>(*it) << " " << get<2>(*it) << endl;
    // }
    // cout << "*------------------------------------------*" << endl;
    vector<vector<pair<int, int>>> LP;
    // cout << "Number of vertices: " << noOfVerticesInOriginalPolygon << endl;
    // LPvj is a list containing pairs (k; vr) where k is the index of a polygon containing vj as one of its vertices and vr is the next vertex to vj in that polygon k, but these pairs are in LPvj only if vjvr is a diagonal,
    for (int i = 1; i <= noOfVerticesInOriginalPolygon; i++) // iterate over all vertices
    {
        // LPvj is a list containing pairs (k; vr) where k is the index of a polygon containing vj as one of its vertices and vr is the next vertex to vj in that polygon k, but these pairs are in LPvj only if vjvr is a diagonal,
        vector<pair<int, int>> LPvj;
        for (auto it = LLE.begin(); it != LLE.end(); it++)
        {
            if ((*get<0>(*it)).start->id == i)
            {
                LPvj.push_back(make_pair(get<1>(*it), (*get<0>(*it)).end->id));
            }
            else if ((*get<0>(*it)).end->id == i)
            {
                LPvj.push_back(make_pair(get<2>(*it), (*get<0>(*it)).start->id));
            }
        }
        LP.push_back(LPvj);
    }
    // print LP
    // cout << "LP: " << endl;
    // for (int i = 0; i < LP.size(); i++)
    // {
    //     cout << "LP" << i + 1 << ": ";
    //     for (int j = 0; j < LP[i].size(); j++)
    //     {
    //         cout << "(" << LP[i][j].first << "," << LP[i][j].second << ") ";
    //     }
    //     cout << endl;
    // }
    int noOfDiag = LLE.size();
    int NP = LLE.size() + 1;
    vector<bool> LDP(NP + 1, true);
    vector<int> LUP;
    // initise LUP with i from 1 to m+1
    for (int i = 0; i <= NP; i++)
    {
        LUP.push_back(i);
    }
    cout << "No of diagonals: " << noOfDiag << endl;
    for (int j = 1; j <= noOfDiag; j++)
    {
        // Get vs and vt from LLE[j]
        Vertex *vs = (*get<0>(LLE[j - 1])).start;
        Vertex *vt = (*get<0>(LLE[j - 1])).end;
        // cout << "vs: " << vs->id << " vt: " << vt->id << endl;
        // cout << "Polygon 1" << endl;
        Vertex *vs1, *vt1, *vs2, *vt2;
        for (int i = 0; i < dcel[get<1>(LLE[j - 1])].vertices.size(); i++)
        {
            if (dcel[get<1>(LLE[j - 1])].vertices[i]->id == vs->id)
            {
                vs1 = dcel[get<1>(LLE[j - 1])].vertices[i];
            }
        }
        // do the same for vt1
        for (int i = 0; i < dcel[get<1>(LLE[j - 1])].vertices.size(); i++)
        {
            if (dcel[get<1>(LLE[j - 1])].vertices[i]->id == vt->id)
            {
                vt1 = dcel[get<1>(LLE[j - 1])].vertices[i];
            }
        }
        // cout << "vs1: " << vs1->id << " vt1: " << vt1->id << endl;
        for (int i = 0; i < dcel[get<2>(LLE[j - 1])].vertices.size(); i++)
        {
            if (dcel[get<2>(LLE[j - 1])].vertices[i]->id == vs->id)
            {
                vs2 = dcel[get<2>(LLE[j - 1])].vertices[i];
            }
        }
        // do the same for vt2
        for (int i = 0; i < dcel[get<2>(LLE[j - 1])].vertices.size(); i++)
        {
            if (dcel[get<2>(LLE[j - 1])].vertices[i]->id == vt->id)
            {
                vt2 = dcel[get<2>(LLE[j - 1])].vertices[i];
            }
        }
        // cout << "vs2: " << vs2->id << " vt2: " << vt2->id << endl;
        // find angle between vs1 and diagonal
        int ang1 = 180 - findAngle(vs1->prev, vs1, vs1->next);
        int ang2 = 180 - findAngle(vs2->prev, vs2, vs2->next);
        int ang3 = 180 - findAngle(vt1->prev, vt1, vt1->next);
        int ang4 = 180 - findAngle(vt2->prev, vt2, vt2->next);
        // cout << "Angle between vs1 and diagonal: " << 180 - findAngle(vs1->prev, vs1, vs1->next) << endl;
        // cout << "Angle between vs2 and diagonal: " << 180 - findAngle(vs2->prev, vs2, vs2->next) << endl;
        // cout << "Angle between vt1 and diagonal: " << 180 - findAngle(vt1->prev, vt1, vt1->next) << endl;
        // cout << "Angle between vt2 and diagonal: " << 180 - findAngle(vt2->prev, vt2, vt2->next) << endl;
        if ((LP[vs->id].size() > 2 && LP[vt->id].size() > 2) || (LP[vs->id].size() > 2 && (ang3 + ang4) <= 180) || (LP[vt->id].size() > 2 && (ang1 + ang2) <= 180) || ((ang1 + ang2) <= 180 && (ang3 + ang4) <= 180))
        {
            Vertex *i1, *i2, *i3, *j1, *j2, *j3;
            j2 = vt;
            i2 = vs;
            // i1 stores the previous vertex of vs in polygon j
            for (int i = 0; i < dcel[get<1>(LLE[j - 1])].vertices.size(); i++)
            {
                if (dcel[get<1>(LLE[j - 1])].vertices[i]->id == vs->id)
                {
                    i1 = dcel[get<1>(LLE[j - 1])].vertices[i]->prev;
                }
            }
            // print i1
            // cout << "i1: " << i1->id << endl;
            // j3 stores the next vertex of vt in polygon j
            for (int i = 0; i < dcel[get<1>(LLE[j - 1])].vertices.size(); i++)
            {
                if (dcel[get<1>(LLE[j - 1])].vertices[i]->id == vt->id)
                {
                    j3 = dcel[get<1>(LLE[j - 1])].vertices[i]->next;
                }
            }
            // print j3
            // cout << "j3: " << j3->id << endl;
            // search in LPvt the only pair containing (p,vs)
            pair<int, int> pvs;
            // cout << vt->id << endl;
            // cout << LP[vt->id - 1].size() << endl;
            for (int i = 0; i < LP[vt->id - 1].size(); i++)
            {
                // cout << "LPvt: (" << LP[vt->id - 1][i].first << "," << LP[vt->id - 1][i].second << ")" << endl;
                if (LP[vt->id - 1][i].second == vs->id)
                {
                    pvs = LP[vt->id - 1][i];
                }
            }
            // cout << "pvs: (" << pvs.first << "," << pvs.second << ")" << endl;
            //  j1 is the previous of vt in the polygon pvs.first
            for (int i = 0; i < dcel[pvs.first].vertices.size(); i++)
            {
                if (dcel[pvs.first].vertices[i]->id == vt->id)
                {
                    j1 = dcel[pvs.first].vertices[i]->prev;
                }
            }
            // print j1
            // cout << "j1: " << j1->id << endl;
            // i3 is the next of vs in the polygon pvs.first
            for (int i = 0; i < dcel[pvs.first].vertices.size(); i++)
            {
                if (dcel[pvs.first].vertices[i]->id == vs->id)
                {
                    i3 = dcel[pvs.first].vertices[i]->next;
                }
            }
            // print i3
            // cout << "i3: " << i3->id << endl;
            // print i1 and i2 coordinates
            cout << "i1: " << i1->id << " i2: " << i2->id << " i3: " << i3->id << endl;
            cout << "Angles bertween i1,i2,i3 and j1,j2,j3: " << findAngle(i1, i2, i3) << " " << findAngle(j1, j2, j3) << endl;
            if (findAngle(i1, i2, i3) <= 180 && findAngle(j1, j2, j3) <= 180)
            {
                NP++;
                cout << "Merge" << endl;
                // merge DCEL[j] and DCEL[pvs.first]
                DCEL merge;
                merge = mergeDCELs(dcel[get<1>(LLE[j - 1])], dcel[pvs.first], i2, j2);
                printDCEL(&merge);
                dcel[get<1>(LLE[j - 1])] = merge;
                LDP[j] = false;
                LDP[pvs.first] = false;
                bablu.insert(pvs.first);
                LUP[j] = LUP[pvs.first] = NP;
                for (int h = 1; h < NP; h++)
                {
                    if (LUP[h] == j || LUP[h] == pvs.first)
                    {
                        LUP[h] = NP;
                    }
                }
            }
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
 
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
 
    for (int i = 1; i < P1.size(); i++)
    {
        cout << P1[i]->x << " " << P1[i]->y << endl;
    }
    cout << "*------------------------------------------*" << endl;
    for (int i = 1; i < m + 2; i++)
    {
        if (bablu.count(i))
        {
            // cout << "merged" << endl;
        }
        else
        {
            // cout << "DCEL " << i << endl;
            printDCEL(&dcel[i]);
        }
    }
    cout << "*------------------------------------------*" << endl;
}