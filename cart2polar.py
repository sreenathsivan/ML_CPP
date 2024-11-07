import math
import numpy as np

def cart2polar(region_size):  #cartesian to polar
    radius=np.zeros((region_size[0],region_size[1]))
    angle=np.zeros((region_size[0],region_size[1]))
    center=[math.ceil(region_size[0]/2),math.ceil(region_size[1]/2)]
    for row in range(region_size[0]):
        for col in range(region_size[1]):
            theta=row-center[0]
            rho=col-center[1]
            
            epsilon = 1e-8
            if(rho>epsilon):
                radius[row][col]=np.log(rho)
            else:
                radius[row][col]=(epsilon)
            angle[row][col]=(theta*(180/math.pi)+180)
            
    return radius,angle      
