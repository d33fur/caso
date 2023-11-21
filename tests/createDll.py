#file for building library for python test

import os

def CreateDll (folderName, fileName, fileComb):
    #fileComb - vector of pairs of file info
    templateCompil = "g++ {fileSource} -o {fileTarget}"
    templateLink = "g++ -shared {objectfile} -o {fileTarget}"

    if os.path.exists(folderName) == False:
        os.makedirs(folderName)
    
    if os.path.exists(fileName):
        os.remove(fileName)
    
    for fso in fileComb:
        if os.path.exists(fso["fileObjectName"]):
            os.remove(fso["fileObjectName"])

    for filePair in fileComb:
        fileSourceName = filePair["fileSourceName"]
        fileObjectName = filePair["fileObjectName"]
        '''cmd = templateCompil.format(
            fileSource = fileSourceName,
            fileTarget = fileObjectName)'''
        
    for filePair in fileComb:
        fileObjectName = filePair["fileObjectName"]

    '''cmd = templateLink.format(
        objectfile = fileObjectName,
        fileTarget = fileName)'''