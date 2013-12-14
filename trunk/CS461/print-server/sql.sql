DROP TABLE Users;
DROP TABLE Projects;
DROP TABLE Tasks;
DROP TABLE Project_Task;
DROP TABLE Manager_Project;
DROP TABLE Task_Requested_By_User;
DROP TABLE Task_Programmer;

CREATE TABLE Users (
  Id VARCHAR(45) NOT NULL,
  Password VARCHAR(45) NOT NULL,
  Type VARCHAR(45) NOT NULL,
  Lang INTEGER,
  PRIMARY KEY(Id)
);

CREATE TABLE Projects (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  Name VARCHAR(45) NOT NULL,
  ManagerId VARCHAR(45) NOT NULL,  
  PRIMARY KEY(Id)
);

CREATE TABLE Tasks (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  Name VARCHAR(45) NOT NULL,
  Status VARCHAR(45) NOT NULL,
  Description LONGTEXT,
  ProjectId INTEGER NOT NULL,  
  PRIMARY KEY(Id)
);

CREATE TABLE Manager_Project (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  ManagerId VARCHAR(45) NOT NULL,
  ProjectId INTEGER NOT NULL,
  PRIMARY KEY(Id)
);

CREATE TABLE Project_Task (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  ProjectId INTEGER NOT NULL,
  TaskId INTEGER NOT NULL,
  PRIMARY KEY(Id)
);

CREATE TABLE Task_Requested_By_User (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  TaskId INTEGER NOT NULL,
  UserId VARCHAR(45) NOT NULL,
  PRIMARY KEY(Id)
);

CREATE TABLE Task_Programmer (
  Id INTEGER NOT NULL AUTO_INCREMENT,
  TaskId INTEGER NOT NULL,
  UserId VARCHAR(45) NOT NULL,
  PRIMARY KEY(Id)
);

INSERT INTO users (Id, Password, Type, Lang) values ('p', 'p', 'Programmer', 0);
INSERT INTO users (Id, Password, Type, Lang) values ('m', 'm', 'Manager', 0);
INSERT INTO users (Id, Password, Type, Lang) values ('mm', 'mm', 'Manager', 0);