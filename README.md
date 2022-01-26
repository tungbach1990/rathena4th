<img src="doc/logo.png" align="right" height="90" />

# rAthena
[![Total alerts](https://img.shields.io/lgtm/alerts/g/rathena/rathena.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/rathena/rathena/alerts/) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/rathena/rathena.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/rathena/rathena/context:cpp) ![GitHub](https://img.shields.io/github/license/rathena/rathena.svg) ![GitHub repo size](https://img.shields.io/github/repo-size/rathena/rathena.svg)
> rAthena is a collaborative software development project revolving around the creation of a robust massively multiplayer online role playing game (MMORPG) server package. Written in C, the program is very versatile and provides NPCs, warps and modifications. The project is jointly managed by a group of volunteers located around the world as well as a tremendous community providing QA and support. rAthena is a continuation of the eAthena project.

[Forum](https://rathena.org/board)|[Discord](https://rathena.org/discord)|[Wiki](https://github.com/rathena/rathena/wiki)|[FluxCP](https://github.com/rathena/FluxCP)|[Crowdfunding](https://rathena.org/board/crowdfunding/)|[Fork and Pull Request Q&A](https://rathena.org/board/topic/86913-pull-request-qa/)
--------|--------|--------|--------|--------|--------

### Table of Contents
1. [Prerequisites](#1-prerequisites)
2. [Installation](#2-installation)
3. [Troubleshooting](#3-troubleshooting)
4. [More Documentation](#4-more-documentation)
5. [How to Contribute](#5-how-to-contribute)
6. [License](#6-license)

## 1. Prerequisites
Before installing rAthena there are certain tools and applications you will need which
differs between the varying operating systems available.

### Hardware
Hardware Type | Minimum | Recommended
------|------|------
CPU | 1 Core | 2 Cores
RAM | 1 GB | 2 GB
Disk Space | 300 MB | 500 MB

### Operating System & Preferred Compiler
Operating System | Compiler
------|------
Linux  | [gcc-5 or newer](https://www.gnu.org/software/gcc/gcc-5/) / [Make](https://www.gnu.org/software/make/)
Windows | [MS Visual Studio 2013, 2015, 2017](https://www.visualstudio.com/downloads/)

### Required Applications
Application | Name
------|------
Database | [MySQL 5 or newer](https://www.mysql.com/downloads/) / [MariaDB 5 or newer](https://downloads.mariadb.org/)
Git | [Windows](https://gitforwindows.org/) / [Linux](https://git-scm.com/download/linux)

### Optional Applications
Application | Name
------|------
Database | [MySQL Workbench 5 or newer](http://www.mysql.com/downloads/workbench/)

## 2. Installation 

### Full Installation Instructions
  * [Windows](https://github.com/rathena/rathena/wiki/Install-on-Windows)
  * [CentOS](https://github.com/rathena/rathena/wiki/Install-on-Centos)
  * [Debian](https://github.com/rathena/rathena/wiki/Install-on-Debian)
  * [FreeBSD](https://github.com/rathena/rathena/wiki/Install-on-FreeBSD)

## 3. Troubleshooting

If you're having problems with starting your server, the first thing you should
do is check what's happening on your consoles. More often that not, all support issues
can be solved simply by looking at the error messages given. Check out the [wiki](https://github.com/rathena/rathena/wiki)
or [forums](https://rathena.org/forum) if you need more support on troubleshooting.

## 4. More Documentation
rAthena has a large collection of help files and sample NPC scripts located in the /doc/
directory. These include detailed explanations of NPC script commands, atcommands (@),
group permissions, item bonuses, and packet structures, among many other topics. We
recommend that all users take the time to look over this directory before asking for
assistance elsewhere.

## 5. How to Contribute
Details on how to contribute to rAthena can be found in [CONTRIBUTING.md](https://github.com/rathena/rathena/blob/master/.github/CONTRIBUTING.md)!

## 6. License
Copyright (c) rAthena Development Team - Licensed under [GNU General Public License v3.0](https://github.com/rathena/rathena/blob/master/LICENSE)

## 7. Expanded AI condition mod

Hi. This is an expanded ai mod that allows to define complex conditions for skill decision with a yml config file.
It was designed to be accessible to everyone, designers, scripters, developpers, even players.
I hope you will have a great deal of fun giving ai a new life.

Installation : copy mob_expanded_ai_conditions_db.yml from import-tmpl onto import folder
There are already some pre-defined entries.

example : inside mob_expanded_ai_conditions_db.yml
```
 - friendrecoverable:
   - or:
     - friend sleep    
     - friend stone
     - friend stun
     - friend freeze

 - selfcellempty:
   - not self safetywall
   - not self pneuma
   - not self landprotector
   - not self icewall
  
 - friendcellempty:
   - not friend safetywall
   - not friend pneuma
   - not friend landprotector
   - not friend icewall
```
Then you have to add skill condition in the usual mob_skill_db.yml, with the label "expanded"
```
1639,Kathryne Keyron@MG_SAFETYWALL,anytarget,12,10,3000,0,10000,yes,self,expanded,selfcellempty,,,,,,,
1639,Kathryne Keyron@MG_SAFETYWALL,anytarget,12,10,3000,0,10000,yes,friend,expanded,friendcellempty,,,,,,,
1637,Margaretha Sorin@PR_STRECOVERY,any,72,1,10000,1000,10000,no,friend,expanded,friendrecoverable,,,,,,,
```
Then, in mob_db you can add the new mods that go particulary well with increased ai : 
```
    Modes:
      SkillOnly: true
      PcSkillBehavior: true     
```
SkillOnly will force the mob to always use skills, making him play more like players do, and PcSkillBehavior will make its skills be either ranged/melee depending on the range and not distance, as players' behave.
Bonus : a crazy asura condition complex, try to figure out what it does ;-)
```
 - asura:
#   - self spiritball >=5 # to be implemented
   - self explosionspirits
   - not enemy safetywall
   - not ignore_neutral
   - nand:
     - enemy neutralresist >=50%
     - enemy ghostarmor >0
   - nand:
     - enemy reflectphy >=10%
     - nxor :
       - enemy neutralresist >=50%
       - enemy ghostarmor >0
```


