##############################################################################
#Example for the virtual field method in linear elasticity
#Date: 31.05.2017
#Author: Patrick Diehl
#Details: http://prod.sandia.gov/techlib/access-control.cgi/2014/1417871.pdf
##############################################################################
import numpy as np
from numpy import linalg
import matplotlib.pyplot as plt
import matplotlib.cm as cm

def cart2pol(x, y):
    rho = np.sqrt(x**2 + y**2)
    phi = np.arctan2(y, x)
    return(rho, phi)

def imagesc(x,y,c,t):
    plt.imshow(c,extent=(x.min(),x.max(),y.min(),y.max()),interpolation='nearest', cmap=cm.gist_rainbow)
    plt.colorbar()
    plt.title(t)
    plt.show()
    
##############################################################################
# Configuration
##############################################################################

#Nomial Elastic Parameters
E = 200000 #MPa=N/mm^2
nu = 0.29

P = 7000 #Applied load in Newton

# Dimesions of the disk
Rad = 50. #mm
t = 2. #mm

# 2D-DIC parameter
Step = 10.
mmPerPix = 2*Rad/2000 # mm / pixel ratio
SmallArea = (Step*mmPerPix)**2

# Constitutice Parameters
InputQ11 = E/(1-(nu**2))
InputQ12 = nu*E/(1-nu**2)

# Show plots of tress and strain
plot = True

##############################################################################
# Code
##############################################################################

# Generate x and y coordiantes in mm
start = -Rad-(Step*mmPerPix/2)
stepsize = Step*mmPerPix
end = Rad+(Step*mmPerPix/2)
i = 0
x = []
while  start + i * stepsize <= end:
    x.append(start + i * stepsize)
    i+=1
    
xMat = np.zeros((len(x),len(x)),dtype=float)
yMat = np.zeros((len(x),len(x)),dtype=float)

for i in range(0,len(x)):
    xMat[:,i] = x[i]
    yMat[:,i] = x[len(x)-i-1]
    
# Convert the cartesian coordiantes to polar coordiantes    
theta = np.zeros((len(x),len(x)))    
r = np.zeros((len(x),len(x)))

for i in range(0,len(x)):
    rpart = []
    thetapart = []
    for j in range(0,len(x)):
        radius , angle = cart2pol(x[j],x[len(x)-i-1])
        rpart.append(radius)
        thetapart.append(angle)
    theta[i,:] = thetapart
    r[i,:] = rpart

r[r > Rad] = 0

# Stress in MPa
sigma1 = (-2.*P/(np.pi*t))*(np.divide(np.multiply((Rad-yMat),xMat**2),((xMat**2)+(Rad-yMat)**2)**2)+np.divide(np.multiply(Rad+yMat,xMat**2),(xMat**2+(Rad+yMat)**2)**2)-(1./(2.*Rad)))
sigma2 = (-2.*P/(np.pi*t))* (np.divide((Rad-yMat)**3,((xMat**2)+(Rad-yMat)**2)**2) +  np.divide((Rad+yMat)**3,((xMat**2)+(Rad+yMat)**2)**2) - (1./(2.*Rad)))
sigma6 = (2.*P/(np.pi*t)) * ( np.divide(np.multiply((Rad-yMat)**2,xMat),((xMat**2)+(Rad-yMat)**2)**2) - np.divide(np.multiply((Rad+yMat)**2,xMat),((xMat**2)+(Rad+yMat)**2)**2))

# Strains
epsilon1 = ((InputQ11/((InputQ11**2)-(InputQ12**2)))*sigma1) - ((InputQ12/((InputQ11**2)-(InputQ12**2)))*sigma2)
epsilon2 = -(InputQ12/((InputQ11**2)-(InputQ12**2))*sigma1) + ((InputQ11/((InputQ11**2)-(InputQ12**2)))*sigma2)
epsilon3 = (2./((InputQ11)-(InputQ12)))*sigma6

r[r > 0] = 1

if plot == True:
    imagesc(xMat,yMat,np.multiply(sigma1,r),"$\sigma_1$")
    imagesc(xMat,yMat,np.multiply(sigma2,r),"$\sigma_2$")
    imagesc(xMat,yMat,np.multiply(sigma6,r),"$\sigma_6$")

    imagesc(xMat,yMat,np.multiply(sigma1,r),"$\epsilon_1$")
    imagesc(xMat,yMat,np.multiply(sigma2,r),"$\epsilon_2$")
    imagesc(xMat,yMat,np.multiply(sigma6,r),"$\epsilon_6$")

# Left-hand side of the matrix
A = np.zeros((2,2))
A[0,0] = np.sum(np.multiply(epsilon2,r))
A[1,1] = np.sum(np.multiply(epsilon2,r))
A[0,1] = np.sum(np.multiply(epsilon1,r))
A[1,0] = np.sum(np.multiply(epsilon1,r))

# Right-hand side of the matrix
B = np.zeros((2))
B[0] = (-2.*P*Rad)/(t*SmallArea)
B[1] = 0.

# Solving the system
x = np.linalg.solve(A,B)

# Comput the erros
M = np.zeros((2))
M[0] = InputQ11
M[1] = InputQ12
Qerror = np.divide(x,M)

EVFM = x[0] *(1.-(x[1]/x[0])**2)
Eerror = (EVFM-E) / E

nuVFM = x[1] / x[0]
nuError = (nuVFM-nu)/nu

print "E =" , EVFM , " nu =" , nuVFM 
print "Eerror =" , Eerror , " nuError =" , nuError 
