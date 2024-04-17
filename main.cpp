#include <iostream>
#include <vector>
#include <functional>


using namespace std;
//The objective function
double f(vector<double>* nPoints){

    double x1 = (*nPoints)[0];
    double x2 = (*nPoints)[1];
    return  (x1 - 3) * (x1 - 3) * (x1 - 3) * (x1 - 3) + (x1  - 3 * x2) * (x1  - 3 * x2);
}
vector<double> operator*(double lambda, vector<double>gradientVector){
    vector<double> answer(gradientVector.size());

    for(int i = 0; i < gradientVector.size(); i++){
        answer[i] = (gradientVector[i] * lambda);
    }
    return answer;
}

vector<double> operator-(vector<double>m_currentPoint, vector<double> gradienVector_lambda){
    vector<double> answer(m_currentPoint.size());

    for(int i = 0; i < m_currentPoint.size(); i++){
        answer[i] = m_currentPoint[i] - gradienVector_lambda[i];
    }
    return answer;
}

class Gradient{
private:
    vector<double> m_startPoint;
    vector<double> m_currentPoint;
    function<double(vector<double>*)> m_f;
    vector<double> point;
    // learning_rate
    double m_lambda;
    // stopping condition
    double m_epsilon;
    int m_nDimensions;
    int m_nIterations;
    double value;
    int n_steps;


public:
    Gradient(){
        m_lambda = 0.0;
        m_epsilon = 0.0;
        m_nDimensions = 0;
        m_nIterations = 1;
        value = 0;
        n_steps = 0;
    }

    void set_Lambda(double lambda){
        m_lambda = lambda;
    }
    void set_epsilon(double epsilon){
        m_epsilon = epsilon;
    }
    void setStartPoint(vector<double> startPoint){

        m_startPoint = startPoint;
        m_nDimensions = static_cast<int>(startPoint.size());
    }
    void setNumberIterations(int numberIterations){
        m_nIterations = numberIterations;
    }
    void setFunction(function<double(vector<double>*)> f ){
        m_f = f;
    }
    double getValue() const {
        return value;
    }
    int getSteps() const{
        return n_steps;
    }
    vector<double> getPoints() const{
        return point;
    }

    void gradientDescent(){

        n_steps = 0;
        double abs_verification = 1;
        m_currentPoint = m_startPoint;
        vector<double> set_points;

        while(abs_verification >= m_epsilon){
            vector<double> gradientVector = computeGradientVector();
            vector<double>gradientVector_lambda = m_lambda * computeGradientVector();
            vector<double> nextPoint = m_currentPoint - gradientVector_lambda;
            abs_verification = verification(m_currentPoint,nextPoint);
            n_steps++;
            m_currentPoint = nextPoint;
            nextPoint.clear();
        }
        value = m_f(&m_currentPoint);
        point = m_currentPoint;
    }

private:
    /* makes the derivation by the following formula:
     * f'(xth_i) = [f(xth + h) - f(xth)] / h
    */
    double computeGradient(int i){
        double h = 0.001;
        vector<double> newPoint = m_currentPoint;
        newPoint[i] += h;
        // a = f(xth + h)
        double a = m_f(&newPoint);
        double b = m_f(&m_currentPoint);
        return (a - b) / h;
    }

    vector<double> computeGradientVector(){
        vector<double> gradientVector;

        for(int i = 0; i < m_nDimensions; i++){
            gradientVector.push_back(computeGradient(i));
        }

        return gradientVector;
    }
    double verification(vector<double> newPoint,vector<double> previousPoint){
        return abs(m_f(&newPoint) - m_f(&previousPoint));
    }

};

int main() {
    Gradient gradient;
    double lambda = 0.01;
    double epsilon = 0.0000001;
    int iterations = 100;
    gradient.set_epsilon(epsilon);
    gradient.set_Lambda(lambda);
    gradient.setStartPoint({10,7});
    gradient.setNumberIterations(iterations);
    function<double(vector<double>*)> func{f};
   gradient.setFunction(func);
    gradient.gradientDescent();
    vector<double> set_OfPoint = gradient.getPoints();
    printf("Valoarea minima atinsa : %.8lf", gradient.getValue());
    printf("\n");
    printf("Numarul de pasi efectuati pentru a atinge valoarea minima : %d", gradient.getSteps());
    printf("\n");
    printf("Pozitia punctului in care se atinge valoarea minima : %lf si %lf", set_OfPoint[0],set_OfPoint[1]);
    //cout << set_OfPoint[0] << " " << set_OfPoint[1];
    return 0;
}
