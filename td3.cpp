#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE

    double *new_array;
    new_array=new double[new_size];

    for (int k=0;k<length;k++) {
        new_array[k]=*array;
        array+=1;
    }

    array-=length;
    delete[] array;

  return new_array; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE

    double* new_array;
    new_array=new double[new_size];

    for (int k=0;k<new_size;k++) {
        new_array[k]=*array;
        array+=1;
    }

    array-=new_size;
    delete []array;

  return new_array; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE

    if (current_size==max_size) {
        max_size+=5;
        array=extend_array(array,current_size,max_size);
    }

    array[current_size]=element;
    current_size+=1;

  return array; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE

    if (current_size>0) {
        current_size-=1;
    }

    if (max_size-current_size>=5) {
        array=shrink_array(array,max_size,max_size-5);
        max_size-=5;
    }

  return array; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
    telemetry=append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
    telemetry=append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
    telemetry=append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &telemetry,
                     int &telemetry_current_size,
                     int &telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE

    for (int k=0;k<tot_telemetries;k++) {
        for (int l=0;l<telemetries_sizes[k];l+=3) {
            telemetry=append_to_array(telemetries[k][l],telemetry,telemetry_current_size,telemetry_max_size);
            telemetry=append_to_array(telemetries[k][l+1],telemetry,telemetry_current_size,telemetry_max_size);
            telemetry=append_to_array(telemetries[k][l+2],telemetry,telemetry_current_size,telemetry_max_size);
        }
    }

    for (int m=0;m<telemetry_current_size;m+=3) {
        int min = m;
        for (int n=m;n<telemetry_current_size;n+=3) {
            if (telemetry[n] < telemetry[min]) min = n;
        }
        double temp[3];
        temp[0] = *(telemetry+min);
        temp[1] = *(telemetry+min+1);
        temp[2] = *(telemetry+min+2);
        *(telemetry+min) = *(telemetry+m);
        *(telemetry+min+1) = *(telemetry+m+1);
        *(telemetry+min+2) = *(telemetry+m+2);
        *(telemetry+m) = temp[0];
        *(telemetry+m+1) = temp[1];
        *(telemetry+m+2) = temp[2];
    }

}
