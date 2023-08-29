# EJP-S: Edged Judger Platform for Students 🌱
EJP-S is a specialized component of the Edged Judger Platform (EJP) designed to empower students in the realm of coding education. Seamlessly integrated with the EJP ecosystem, EJP-S enables students to get assignments, submit their answers, ans even manage their groups.

## Table of Contents

- [Highlights 🌟](#Highlights-)

- [Getting Started 📚](#Gettting-started-)
    - [Prerequisites](#Prerequisites)
    - [Installation](#Installation)
    - [Usage](#Usage)

- [Contributing 🤝](#Contributing-)

- [License 📄](#License-)

- [Details](#Details)
    - [repo](#repo)
        - [command](#command)
    - [problem](#problem)
        - [command](#command-1) <br/><br/>


## Highlights 🌟

- **Get Repositories**: Get your information of remote repositories.

- **Test Code**: Test your code with your executable file.

- **Submit Answer**: Submit answer. <br/><br/>

## Gettting Started 📚

### Prerequisites

- Ensure the the main EJP system set up and running

- Requires Linux OS (Windows & Mac support coming soon)

### Installation 

1. Clone the EJP-S repository.
    ```
        git clone https://github.com/EdgeCrafters/ejp-s.git
    ```

2. Navaigate to the cloned directory and run the installation script.
    ```
        cd ejp-s
        ./install.sh
    ```

3. Follow the on-screen prompts to complete the installation.

### Usage

1. Launch EJP-S from the command line:
    ```
        ejp-s [category] [command] [options]
    ```
    <br/>
    Parameters

    - [category]: The category to which the command belongs (e.g., repo, problem).
    - [command]: The specific action you want to perform (e.g., show, get, submit, test, testcase).
    - [options]: Additional parameters or flags that modify the command. (e.g., -h(host), -l(location)). See below for more information about parameters.

2. Use the menu options to get information of remote repositories, submit your answers, or sync with EJP-T, and EJP-BE.

3. For a list of specified available commands and options, use:
    ```
        ejp-s [category]
    ```
<br/><br/>

## Contributing 🤝

We welcome contributions from the community! Feel free to fork the repository, make your changes, and submit a pull request. For more details, check out our [contribution guidelines](#).
<br/><br/>

## License 📄

EJP-S is part of the EJP project and follows the same MIT license.

## Details

### repo

You can controll information of your repositories.
    
```
    ejp-s repo [command] [options]
```
<br/>

#### ```[command]```

0. ```show```: see information of your repositories.
    <br/>
    - ```-h```: url of server
    <br/>

```
    ejp-s repo show -h [host address]
```
```
    ejp-s repo show -h localhost:8000
```
<br/>

1. ```get```: get information of your repositories. 
    <br/>
    - ```-h```: url of server
    <br/>

```
    ejp-s repo get -h [host address]
```
```
    ejp-s repo get -h localhost:8000
```
<br/>

2. ```clean```: clean information of your repositories that saved local directory.
    <br/>
    ```
        ejp-s repo clean
    ```
    <br/>

### problem

You can submit and test your answer.<br/>
You can show testcases that opened or hidden.

#### ```[command]```

0. ```show```: see information of problems.
    <br/>
    ```
        ejp-s problem show
    ```
    <br/>

1. ```submit```: submit your answer. 
    <br/>
    - ```-h```: url of server.
    - ```-l```: path to answer file. (compiled file)
    <br/>

```
    ejp-s problem submit -h [host address] -l [path to answer file]
```
```
    ejp-s problem submit -h localhost:8000 -l /myAnswers/answer1
```
<br/>

2. ```test```: test your answer with testcase that published by your instructor.
    <br/>
    - ```-l```: path to answer file. (compiled file)
    <br/>
```
    ejp-s problem test -l [path to answer file]
```
```
    ejp-s problem test /myAnswers/answer1
```
<br/>

3. ```testcase```: see information for testcases.
    <br/>
    ```
        ejp-s problem testcase
    ``` 
    <br>