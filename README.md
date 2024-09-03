
## **How to Create an SDK to Extract Rocket League Data: Reverse Engineering in Unreal Engine Games**

In this article, we will use reverse engineering concepts and techniques applied to Rocket League and Unreal Engine 3 to create an SDK (Software Development Kit) to obtain the following information: the names of the teams, all the members of each team, and the score. Here, you will learn:

* What is an SDK?

* How things are structured in Unreal Engine

* How to obtain the GObject and GName addresses using IDA

* How to use these addresses to get the information we want.

![](https://cdn-images-1.medium.com/max/2000/1*BQnnK_WQhp5KPN4XVnsYzg.jpeg)

## Warning

This article is intended solely for educational and informational purposes. The techniques and methods described here are provided to help readers understand the underlying mechanics of game software and are for learning purposes only.

Any modifications or techniques discussed here are strictly intended for offline use. We strongly discourage using these techniques to gain any unfair advantage, interfere with online gameplay, or harm the experience of other players in any game.

## 1. What is an SDK?

In the context of our project, which aims to extract team and player data from Rocket League in real time, an SDK (Software Development Kit) is a collection of headers containing class definitions, structures, and functions used by the game, providing access to them.

To build these headers, we need to **reverse engineer** **Rocket League** and **Unreal Engine**, and two structures will be essential for this: GObject and GNames.

## 2. Unreal Engine Structure

In Unreal Engine 3, there are two global structures: GObject and GNames. They are used to manage all the objects and names in the game.

### 2.1 GObjects - Global Object Array

GObjects is an instance of a class called TUObjectArrayand contains pointers to all UObject in the game. UObject represents all objects, such as actors, components, assets, etc.

The class that represents TUObjectArray is defined as:

    struct FUObjectItem
    {
     class UObject* Object;
    };
    
    class TUObjectArray {
    public:
     FUObjectItem* Objects; // Pointer to array of pointers to UObject
     int32_t NumElements;   // Number of current elements in array
    
    public:
     inline int32_t Num() const
     {
      return NumElements;
     }
    };

Therefore, to access a UObject from an index, simply do:

    UObject* GetObjectByIndex(int index)
    {
     return GObjects->Objects[index].Object;
    }

Similarly, if we want to get the number of current elements, which is in the NumElements member, we will do:

    uint32_t GetObjectCount()
    {
      return GObjects->Num();
    }

I will present the complete definition of the UObject for reference only, but we will use only the members Outer, NameIndex, and Class, which will be explained in more detail later, along with the UClass class. The other members will not be covered here as they are beyond the scope of the article.

    class UObject
    {
    public:
     struct FPointer VfTable; //0x0000
     struct FPointer* HashNext; //0x0008
     uint64_t UObjectFlags; //0x0010
     struct FPointer* HashOuterNext; //0x0018
     struct FPointer* StateFrame; //0x0020
     class UObject* Linker; //0x0028
     int64_t LinkerIndex; //0x0030
     uint32_t ObjectInternalInteger; //0x0038
     uint32_t NetIndex; //0x003C
     class UObject* Outer; //0x0040
     uint32_t NameIndex; //0x0048
     char pad_004C[4]; //0x004C
     class UClass* Class; //0x0050
     class UObject* ObjectArchtype; //0x0058
    };

### 2.2 GNames — Global Names Array

GNames is an instance of a class called FNamePool and contains pointers to all FNameEntry, which represents a string. I will define all this shortly.
This array stores all the unique names used in the game to avoid string duplication and optimize memory usage. For example, if two different UObject have the same unique name, instead of allocating memory for both names, it is enough to point to the same index of the FNamePool that represents their name, avoiding string duplication.

The structure that represents FNameEntry is defined as:

    class FNameEntry
    {
    public:
     int64_t Flags; //0x0000
     int32_t Index; //0x0008
     char pad_000C[12]; //0x000C
     wchar_t Name[0x400]; //0x0018
    }; //Size: 0x0338
    
    class GNames
    {
    public:
     class FNameEntry* FNamesArray[0x285F6]; //0x0000
    };

## 3. How to Obtain the Addresses

You can skip this chapter if you already have the offsets or have another way of getting them. There are also several videos on the internet teaching how to obtain them using pattern scanning that are simpler.

We can use several methods to obtain the addresses of GObjects and GNames, from public pattern scans to reverse engineering the binary using a disassembler.

It is interesting for the reader to try to find them on their own, but I will briefly show how I found them using a disassembler called IDA64 for static analysis and a structure dissector called ReClass.net to validate if it is correct or not.

### 3.1 Using IDA64

With IDA, we can perform a static analysis of a binary file. Several methods can be used here; generally, a .pdb file from an engine version identical to the one the game uses is employed. By performing comparative analyses, it is possible to find some function that uses some structure or class that you want to find.

In our case, we want to find GNames and GObjects. There is a native function known as IsValid that checks whether an object is valid or not by performing a series of checks.

Here it is in IDA:

![](https://cdn-images-1.medium.com/max/2000/1*2SVOmxhWH82BspPixj7vDQ.png)

On line 37, it performs the first checks: if the index is negative or greater than or equal to an unknown qword, the object is invalid. Here, it is easy to notice that qword_24BF4E0 refers to the number of objects in the game so far; with just this, it would already be possible to obtain GObjects.

Further down in the same function:

![](https://cdn-images-1.medium.com/max/2000/1*XJ1wy9exFosyP5kYrePwgQ.png)

On line 110, it retrieves an object from the array qword_24BF4D8 and checks if there is any other object in the same region. With this analysis, we can already state that this qword is our GObjects.

Now we need GNames. Going back further up in the same function:

![](https://cdn-images-1.medium.com/max/2000/1*JQF_2aeBfSi88SPm2RE8Sw.png)

On line 51, the function sub_2E9FB0 receives a string variable as a parameter and an array qword_24BF490. This string, further down, is used on line 90 to print the object's name:

![](https://cdn-images-1.medium.com/max/2000/1*Z0-nHmvdyLsPmwBdeo6yDQ.png)

Based on this, we can state that the array qword_24BF490 is GNames.

In the next chapter, I will demonstrate how to validate these two offsets using the structure dissector ReClass.net.

## 4. Understanding the Structure with ReClass.net

Another tool that will be widely used here is a structure dissector known as ReClass.net; it looks like this:

![](https://cdn-images-1.medium.com/max/2000/1*XQ-ZAi8B_ilmIoJOYrH1Vw.png)

This tool will be very useful for us to better understand how things are structured in the game; we can access values in real-time and rename members and structures. It also has a functionality that will help us generate the ready-made class code in C++.

I will not explain how to use ReClass itself; I will only show how to validate the offsets we need. There are many good resources online if you are not familiar with the tool.

### 4.1 Validating GNames

First, let’s check if the offset for GNames is correct. To do this, simply enter <{Process Name}>+{0xOffset}.

By changing the type of the first offset (0x0000) to a pointer, to access where the HEAP is pointing, we see the following:

![](https://cdn-images-1.medium.com/max/2000/1*czoaWdLfb2y3IPmKgE1O5A.png)

We have an array where each offset points to another memory address. As we saw in the definition of GNames presented in subsection 2.2, the first offset points to FNamesArray, which is of type FNameEntry, where the name is at offset 0x018. To see if it is correct, let's see where the first FNameArray points:

![](https://cdn-images-1.medium.com/max/2000/1*nH7UMLSu08vlZjHOPAdpQg.png)

In Unreal Engine, GNames at index 0 will always be "None". When we look at offset 0x008, which according to our definition is the index, we see it is 0, and the Name at 0x0018 is "None"; therefore, our offset for GNames is correct.

Let's rename and adjust the types in ReClass to make it more organized, resulting in the following:

![](https://cdn-images-1.medium.com/max/2000/1*jwTstk17FFyNIwgP9G8-Yg.png)

### 4.2 Validating GObjects

Okay, we have the correct GNames; now we need to validate our GObjects. We will enter the offset in ReClass just like we did with GNames:

![](https://cdn-images-1.medium.com/max/2000/1*g6DVk2OdClIIxDX-rz-75Q.png)

We will set the correct types, following the UObject definition presented in subsection 2.1, and it will look like this:

![](https://cdn-images-1.medium.com/max/2000/1*yBJl9SG_VjfrOBssm-1HgA.png)

Perfect! We have GObjects as well. To find out the name of this object, let's take the NameIndex at offset 0x48, which has the value 16713, and put it in GNames that we defined earlier:

![](https://cdn-images-1.medium.com/max/2000/1*U5qmtOzN4jrcaEK0M7s7Zg.png)

Now we know that the name of the object at index 0x0 is ‘Config_ORS’. This way, we can find the name of any UObject in the game.

## 5. Building the SDK

With GObjects and GNames validated, we have what we need to complete the first step of our SDK: extracting all the objects loaded in the game so far. I say "so far" because Unreal Engine loads objects dynamically, as needed.

### 5.1 Extracting the Name of All Objects

To find out the number of loaded objects, we will access the NumElements member of the FUObjectArray class, defined earlier in subsection 2.1, and iterate through all the objects, obtaining the name associated with the NameIndex of each one:

    for (int i = 0; i < UObject::GetObjectCount(); ++i)
    {
      UObject* CurrObject = UObject::GetObjectByIndex(i);
      if (CurrObject)
      {
       wprintf(L"%i | [%p] %ls\n", i, CurrObject, GNames::GetNameByIndex(CurrObject->NameIndex).c_str());
      }
    }

![](https://cdn-images-1.medium.com/max/2000/1*kNF-QNgp-JDDcRQpjkW1iw.png)

As we can see, the object at index 0 has the name ‘Config_ORS’, as we checked manually before.

Okay, we managed to extract the names of all objects in the game. Now, we need to extract the full name of the object and its respective type. For this, we also need to iterate through the Outer of each UObject. The Outer represents the UObject preceding it. Let's take the address of Config_ORS that we just extracted and put it in ReClass to understand it better:

![](https://cdn-images-1.medium.com/max/2000/1*JJB_boDHjCkmdUHAs9fDUA.png)

The name index of this object is at index 0x48 (marked as UObject Name Index in the image above), which represents the name '**Config_ORS**'. Its Outer is at index 0x48 of Outer (marked as Outer Name Index in the image above), which represents '**Core**' if you check in GNames. Combining the two, we have: Core.Config_ORS as the full name of this UObject.

Note that the Outer name always comes first, so the process of constructing the full name of a UObject must happen in reverse. A UObject can have multiple Outer.

This process should continue as long as Outer points to a valid address. We will implement a GetFullName function that will do this:

    std::wstring UObject::GetName()
    {
     return std::wstring(FNamePool->FNamesArray[NameIndex]->Name);
    }
    
    std::wstring UObject::GetFullName()
    {
     std::wstring fullName = this->GetName();
    
     for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
     {
      fullName = (uOuter->GetName() + L"." + fullName);
     }
    
     fullName = this->Class->GetName() + L" " + fullName;
    
     return fullName;
    }

Updating our initial code with this:

    for (int i = 0; i < UObject::GetObjectCount(); ++i)
     {
      UObject* CurrObject = UObject::GetObjectByIndex(i);
      if (CurrObject)
      {
       wprintf(L"%i | [%p] %ls\n", i, CurrObject, CurrObject->GetFullName().c_str());
      }
     }

![](https://cdn-images-1.medium.com/max/2000/1*j-vL_0KqXj-9xNLpRNDK6g.png)

Now we have the full names and addresses of each object in the game!

### 5.2 Extracting Class Members

To extract the members, we need to find out where they are stored within the corresponding UClass. We can do this through static analysis using IDA or dynamic analysis with ReClass to try to infer what each part represents.
Let's take a UObject from our list of objects and put its address in ReClass:

![](https://cdn-images-1.medium.com/max/2000/1*XFO5CmdW5cwpe9FV0rHVWA.png)

This is the object “**Class Engine.ActorFactoryAI**” at index 189. I recommend the reader open it in ReClass and try to find out on their own where the members could be, using IDA as a guide.

Within this object, let’s open its Class, which is a class called UClass. It has two more classes that it inherits from UObject: UStruct and UProperty.

The UStruct is defined as:

    class UStruct : public UObject
    {
    public:
     char pad_0000[0x20]; //0x0000
     class UStruct* SuperField; //0x0008
     class UStruct* Children; //0x0010
     uint32_t PropSize; //0x0018
     char pad_001C[4]; //0x001C
    };

This class is related to the hierarchy of the current UClass.

The SuperField member points to the parent class of the current UClass.

Since the UObject we took is a class (as seen by the name "Class" at the beginning of its full name), let's set it in ReClass as UClass and define the SuperField according to the UStruct definition above:

![](https://cdn-images-1.medium.com/max/2000/1*-M4uyQ3DaZXLU8XY2xy2ig.png)

The first NameIndex of UClass at offset 0x48, which has the value 17144, represents "**ActorFactoryAI**," which is the name of the object we used as an example.

The second NameIndex at offset 0x48 within SuperClass has the value 17140, which, if we look in GNames, has the name "**ActorFactory**."

![](https://cdn-images-1.medium.com/max/2000/1*YeSatFac9BYDMm-2wV6kqw.png)

As we have seen, the SuperField represents the parent inheritance of the current object. Therefore, it would be the same as writing:

    public ActorFactoryAI : public ActorFactory
    {
    public:
      // Members
    };

The UProperty is defined as follows:

    class UProperty : public UStruct
    {
    public:
     int32_t PropOffset; //0x0098
     char pad_009C[4]; //0x009C
     class UProperty* Next; //0x00A0
     char pad_00A8[32]; //0x00A8
    };

This class represents a property or member of a class and stores information such as: where in memory this member is located (offset), its name (since it inherits from UObject through UStruct), and the next member of the class.

Since it inherits from UStruct, it starts immediately after it in memory. Later on, this will be better visualized within ReClass, with the names and types already defined.

The description of the members is as follows:

* PropOffset - Represents the region in memory (offset) where the current member is located in the class.

* Next - The next member.

Finally, the definition of UClass is as follows:

    class UClass : public UProperty
    {
    public:
     char pad_0000[0x48]; //0x0098
     class UProperty* Members; //0x110
    };

To get the complete list of members of a class, we need to perform the following procedure:

 1. Get the address of the desired class (we will use the GetFullName function we implemented earlier).

 2. Interpret this address as a UClass.

 3. Access the address of the members at offset 0x110.

 4. Get the name of the first member using GetFullName.

 5. Go to the next member pointed to by Next.

 6. Repeat step 4 until Next points to a valid address.

Let’s do this manually using ReClass to understand how it works. Below is an image of ReClass in the class Class Engine.PlayerController at index 230:

![](https://cdn-images-1.medium.com/max/2000/1*o7cOyjhOUa9VYn1QSAVLnQ.png)

Going down to offset 0x110, we access the address that Members is pointing to and get its name in GNames:

![](https://cdn-images-1.medium.com/max/2000/1*2v0vRhebJ9T7TLs3TKLdfQ.png)

15530 in GNames represents "**Player**". The PropOffset at offset 0x098 has the value 0x478. Translating all this: the first member of the Engine.PlayerController class is Player and is at offset 0x478 of the class. To know the next member, just go to the address pointed to by Next and repeat the process:

![](https://cdn-images-1.medium.com/max/2000/1*jP4SKf2vRFxOZX-FRmyYSg.png)

Following the Next, we have 36203 as the NameIndex. In GNames, this represents "**PlayerCamera**", and it is at offset 0x480. This is the second member of our class.

Let's create an algorithm to perform this process while Next points to a valid address:

    void UClass::PrintMembers()
    {
     for (UClass::UProperty* Member = this->Members; Member; Member = Member->Next)
     {
      wprintf(L"%ls - 0x%08X\n", Member->GetFullName().c_str(), Member->PropOffset);
     }
    }.

We managed to print the name and offset of all the members of a class, but… we haven’t implemented a way to get the address of a specific class by its name yet.

Let’s do it in a simple way: iterate through all UObjects and compare if the substring we are searching for is contained in the name of the object. If found, it returns the address interpreted as UObject.

    UObject* UObject::GetObjectByName(const wchar_t* ClassName)
    {
     uint32_t currentIndex = 0;
     while (currentIndex < GetObjectCount())
     {
      UObject* Object = GetObjectByIndex(currentIndex);
      if (Object && Object->GetFullName().find(ClassName) != std::string::npos)
      {
       return Object;
      }
      currentIndex += 1;
     }
     return nullptr;
    }

Now we just need to get the address of the desired class, interpret it as UClass, and call the function we just implemented:

    UClass* Class = reinterpret_cast<UClass*>(UObject::GetObjectByName(L"Class Engine.PlayerController"));
    Class->PrintMembers();

![](https://cdn-images-1.medium.com/max/2000/1*tVQpSoULBnGNvTv9bsBfyg.png)

And there you have it — all the members and their offsets of the Engine.PlayerController class! :)

Now that we have a way to identify all objects and obtain the members and offsets of any class we want, let's move forward.

## 6.0 Getting Player Names

To get the names of the players, we need to identify which class holds this information. This can be done in several ways.

One way is simply to look at all the objects we extracted and search for a keyword, such as “**PlayerName**,” “**Name**,” etc. However, this method is exhaustive and **not** very effective, especially for large games.

Another way is to hook a function called ProcessEvent. This function is responsible for processing all game events; basically, every call involving UObjects goes through it. Therefore, just hook it to intercept the UObjects that are associated with the calls when you perform an action involving something you want. For example, if you want to get the team names, you can intercept the ProcessEvent when entering a match, and this will filter only the classes responsible for that action. This method is a bit complex, and I will not go into it in depth.

A third method, which may be a bit simpler, is to use software like **Cheat Engine**, create an offline match with bots, and search for the memory address that stores a player's name. After that, we can place this address in ReClass and identify its UClass, and by NameIndex, we will have the class to which that value belongs.

I will not demonstrate how to obtain this because the article would become too long and stray from its purpose, but I recommend the second or third method, depending on your goals. You can find good material on this on the internet. For the purpose of this article, I recommend the third method.

### 6.1 Reconstructing the Player’s Class

After obtaining the memory region that holds a player’s name, I got the following name in UClass: GFxData_PRI_TA. Looking it up in the objects we extracted, we have the following object: Class TAGame.GFxData_PRI_TA.

Extracting its members with the implementation we did earlier, we have the following members:

    UClass* Class = reinterpret_cast<UClass*>(UObject::GetObjectByName(L"Class TAGame.GFxData_PRI_TA"));
    Class->PrintMembers();

    StrProperty GFxData_PRI_TA.PlayerName - (0x00000098)
    StrProperty GFxData_PRI_TA.PlayerIDString - (0x000000A8)
    StrProperty GFxData_PRI_TA.Platform - (0x000000B8)
    ObjectProperty GFxData_PRI_TA.Avatar - (0x000000C8)
    IntProperty GFxData_PRI_TA.Team - (0x000000D0)
    IntProperty GFxData_PRI_TA.Score - (0x000000D4)
    IntProperty GFxData_PRI_TA.Goals - (0x000000D8)
    IntProperty GFxData_PRI_TA.Assists - (0x000000DC)
    IntProperty GFxData_PRI_TA.Saves - (0x000000E0)
    IntProperty GFxData_PRI_TA.Shots - (0x000000E4)
    IntProperty GFxData_PRI_TA.Ping - (0x000000E8)
    ...
    // Some more members below

Since this class has many members, I cut it off only until Ping, for better visualization.

Of all these members, let's declare only the ones we want to use, which will be PlayerName and Team, as follows:

    class UGFxData_PRI_TA : public UObject
    {
    public:
     char pad_0000[0x38]; // 0x60
     wchar_t* PlayerName; // 0x98
     char pad_0001[0x30]; // 0xA0
     uint32_t Team; // 0xD0
    };

The pad_xxxx members fill the space of other members that we are not going to use to achieve the correct alignment and access the right memory region we want. Since we are inheriting from UObject, which ends at offset 0x58, this class will start at offset 0x60 (0x58 + 0x08 = 0x60). As we have seen in the members, PlayerName is at offset 0x98, so doing 0x98 - 0x60 = 0x38, we set the size of pad_0000 to 0x38, and its next member will be at offset 0x98. This is useful so we don’t have to declare all the class members until we reach the one we want.

### 6.2 Reconstructing the Team’s Class

To get the team’s class, I also used the third method I presented, searching for a team name and identifying its region in memory. With this, I arrived at the following class: GFxData_TeamInfo_TA. Searching for this class in our list of objects, we have: "Class TAGame.GFxData_TeamInfo_TA".
Extracting its members, we get the following list:

    UClass* Class = reinterpret_cast<UClass*>(UObject::GetObjectByName(L"Class TAGame.GFxData_TeamInfo_TA"));
    Class->PrintMembers();

    IntProperty GFxData_TeamInfo_TA.TeamID - (0x00000090)
    StrProperty GFxData_TeamInfo_TA.TeamName - (0x00000098)
    StructProperty GFxData_TeamInfo_TA.TeamColor - (0x000000A8)
    IntProperty GFxData_TeamInfo_TA.Score - (0x000000AC)
    ObjectProperty GFxData_TeamInfo_TA.TeamLogo - (0x000000B0)
    IntProperty GFxData_TeamInfo_TA.SeriesScore - (0x000000B8)
    ObjectProperty GFxData_TeamInfo_TA.Team - (0x000000C0)
    IntProperty GFxData_TeamInfo_TA.Difficulty - (0x000000C8)
    ...
    // Some more members below

Just like we did with the players’ class, we will declare only the members that interest us, as follows:

    class GFxData_TeamInfo_TA : public UObject
    {
    public:
     char pad_0000[0x30]; // 0x60
     uint32_t Team; // 0x90
     class FName* TeamName; // 0x98
     char pad_0001[0xC]; // 0xA0
     uint32_t Score; // 0xAC
    };

## 7.0 Obtaining Class Instances

Now we have the classes and their definitions, but we don’t have their instances. As we have seen, practically everything in Unreal Engine is a UObject, and class instances will be no different.
To get the instances, we need to reconstruct a native Unreal Engine function known as IsA. It performs the following procedure:

 1. Takes the static address of the desired UClass as a parameter.

 2. Iterates through all UObjects, checking if Class points to the static address passed.

 3. If so, it is an instance.

 4. If not, checks if SuperField points to the static address passed.

 5. If so, it is an instance.

 6. If not, repeats step 4 until SuperField is an invalid address.

 7. If not found, it is not an instance.

The implementation of this function in our SDK would look like this:

    bool UObject::IsA(UClass* uClass)
    {
     for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = uSuperClass = static_cast<UClass*>(uSuperClass->SuperField))
     {
      if (uSuperClass && uSuperClass == uClass)
      {
       return true;
      }
     }
     return false;
    }

Now, we need to iterate through all the objects and save in a vector all the UObjects that IsA identifies as an instance:

    std::vector<UObject*> UObject::GetInstancesByClass(UClass* uClass)
    {
     std::vector<UObject*> instances;
    
     for (int i = 0; i < UObject::GetObjectCount(); i++)
     {
      UObject* currObject = UObject::GetObjectByIndex(i);
    
      if (currObject && currObject->IsA(uClass))
      {
       instances.push_back(currObject);
      }
     }
     return instances;
    }

Passing as a parameter the class we obtained earlier for the players, for example, we will get the following return (after entering an offline 4x4 match):

![](https://cdn-images-1.medium.com/max/2000/1*SjrkwN3EZKnSbAEaxC_Acw.png)

Note that **9 instances were found** — one for each player and one for the class template (usually the first one).

Placing one of these instances in ReClass:

![](https://cdn-images-1.medium.com/max/2000/1*m45GLdni8Y1kN-bzw2ldsw.png)

![](https://cdn-images-1.medium.com/max/2000/1*6izYeU6rufSuscTuUe-Tcg.png)

We can see at offset 0x0098 the player’s name, as shown in the definition we made in subsection 6.1.

Now we just need to get the team instances, relate each player to a team, and separate them for a more organized visualization. We achieved the following result:

![](https://cdn-images-1.medium.com/max/2000/1*BQnnK_WQhp5KPN4XVnsYzg.jpeg)

And that’s it! We developed an SDK from scratch with everything needed to achieve our goal.
