import urllib.request 
from PIL import Image 
import numpy as np 
from patchify import patchify
import matplotlib.pyplot as plt
import cv2
import matplotlib.pyplot as plt
from cart2polar import cart2polar
from getbin import getbin
from sklearn.preprocessing import MinMaxScaler
from scipy.io import savemat
import math



def call_ssd(patch,region,alpha,center_patch):
    # patch_size=patch_size.shape
    region_size=region.shape
   
    SSD_region=np.zeros((region_size[0],region_size[1]))
 

    for row in range(center_patch[0]+1,region_size[1]-center_patch[1]):
        for col in range(center_patch[1],region_size[1]-center_patch[1]):
            temp = region[row-center_patch[0]:center_patch[0]+row, col-center_patch[1]:center_patch[1]+col, :]
            
            diff=temp-patch
            SSD_region[row][col]=np.sum(diff**2)
            SSD_region[row][col] = np.exp(-alpha*SSD_region[row][col]);  
            
    return SSD_region

def get_self_sim_vec(SSD_region, bin, vec_size):
    self_similarities_vec=np.zeros((vec_size))
 
    
    num=0
    for m in range(20):
        for n in range(4):
            temp=bin[m][n]
           
            max_value=0
            temp_size=len(temp)
          
            for loc in range(temp_size):
                row,col=temp[loc]
             
                max_value=max(max_value,SSD_region[row][col])
                
         
            self_similarities_vec[num]=max_value
            num=num+1
    return self_similarities_vec
   

def com_Self_Similarities(imgRgb,region_size,patch_size,bin):
    lab_image = cv2.cvtColor(imgRgb, cv2.COLOR_BGR2LAB)
    
    lab_size=lab_image.shape
    vec_size=80
    alpha=1/(85**2)
    self_similarities=np.zeros((lab_image.shape[0],lab_image.shape[1],vec_size))

    center_region=[math.floor(region_size[0]/2),math.floor(region_size[1]/2)]
    center_patch=[math.floor(patch_size[0]/2),math.floor(patch_size[1]/2)]
    count=center_region[0]+1 ,lab_size[0]- center_region[0]
    for row in range(center_region[0]+1 ,lab_size[0]- center_region[0]):
        print(f" {row}>{count}")
        for col in range(center_region[1]+1,lab_size[1]-center_region[1]):
            patch = lab_image[row-center_patch[0]:row + center_patch[0], col-center_patch[0]:col + center_patch[0], :]
            region = lab_image[row - center_region[0]:row + center_region[0] + 1,
                               col - center_region[1]:col + center_region[1] + 1, :]
            SSD_region=call_ssd(patch,region,alpha,center_patch)
            vec=get_self_sim_vec(SSD_region, bin, vec_size)
            # print(vec.shape)
            vec=vec.reshape(-1,1)

            scaler = MinMaxScaler()
            scaler.fit(vec)
            vec=vec.reshape(-1)
            # print(vec)
            
            self_similarities[row, col, :] = vec
            # print(self_similarities.shape)
            # self_similarities[r]
            # print(a)
            
    return self_similarities   
            

    
        

# print(lab_image.shape)
# self_similarities=np.zeros((lab_image.shape[0],lab_image.shape[1],45))
region_size = [46, 37]
patch_size = [5, 5]

radius,angle= cart2polar(region_size) 
bin=getbin(radius,angle,region_size)



n_img=5

for i in range(n_img+1):
    name = f'Input{i+1}.jpg'
    print(name)
    image = cv2.imread(f'{name}', 1)
    # plt.imshow(image)
    # plt.show()
    imgRgb=cv2.resize(image, (0, 0), fx = 1/3, fy = 1/3)
    self_similarities=com_Self_Similarities(imgRgb,region_size,patch_size,bin)
    print(self_similarities)
    np.save(f'mat{i+1}', self_similarities)
    
import numpy.matlib 
width=1
height=1
center_sub = [math.floor(width/2), math.floor(height/2)]
p = 1
self_similarities1=np.zeros((1,n_img))
for m in range(n_img):
    name = f'mat{m+1}.npy'
    data=np.load(f'{name}')
    img_size1=data.shape
    name = f'Input{m+1}.jpg'
    
    self_similarities1=data
  
    
    src_img = cv2.imread(f'{name}', 1)
    imgRgb=cv2.resize(src_img, (0, 0), fx = 1/3, fy = 1/3)
    sig_score_img = np.zeros((img_size1[0],img_size1[1]))
    for row1 in range(center_sub[0]+1,img_size1[0]-center_sub[0]-1):
        for col1 in range(center_sub[1]+1,img_size1[1]-center_sub[1]-1):
            # sub1 = self_similarities1[row1-center_sub[0]:row1+center_sub[0],col1-center_sub[1]:col1+center_sub[1]]
            sub1 = self_similarities1[row1 - center_sub[0]:row1 + center_sub[0], col1 - center_sub[1]:col1 + center_sub[1], :]

            max_match = np.zeros((1, n_img-1))
            num_img=1
            match_score = np.array(n_img+1)
            for n in range(n_img):
                if n!=m:
                    temp = savemat(f'self_similarities0{n}.mat', return_dict=True)
                    self_similarities2 = temp['self_similarities']
                    Path = (f'mat{n+1}.npy')
                    temp = np.load(Path)
                    self_similarities2 = temp
                    print(self_similarities2.shape[1])
                   
                    temp1 = np.tile(sub1, (self_similarities2.shape[0], self_similarities2.shape[1], 1))
                    temp2 = -1 * np.sum((self_similarities2 - temp1) ** 2, axis=2)

                    max_match[1,num_img] = max(max(temp2))
                    match_score[num_img] = np.reshape(temp2,[],1);  
                    
                    
                    
                    
            
             
        
        pass
        
    print(sig_score_img)
   
    

# Loading the image

# imgRgb=cv2.resize(image, (0, 0), fx = 1/3, fy = 1/3)

# self_similarities=com_Self_Similarities(imgRgb,region_size,patch_size,bin)
# print(self_similarities)
# np.save('mat', self_similarities)



# print(bin)
# center_region=[math.floor(region_size[0]/2),math.floor(region_size[1]/2)]
# center_path=[math.floor(patch_size[0]/2),math.floor(patch_size[1]/2)]
# print(center_path)
# for row in 