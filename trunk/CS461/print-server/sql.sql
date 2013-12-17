SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
CREATE SCHEMA IF NOT EXISTS `project461` DEFAULT CHARACTER SET utf8 ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `project461`.`tasks`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`tasks` ;

CREATE TABLE IF NOT EXISTS `project461`.`tasks` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT,
  `Name` VARCHAR(45) NOT NULL,
  `Status` VARCHAR(45) NOT NULL,
  `Description` LONGTEXT NULL DEFAULT NULL,
  `ProjectId` INT(11) NOT NULL,
  PRIMARY KEY (`Id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `project461`.`users`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`users` ;

CREATE TABLE IF NOT EXISTS `project461`.`users` (
  `Id` VARCHAR(45) NOT NULL,
  `Password` VARCHAR(45) NOT NULL,
  `Type` VARCHAR(45) NOT NULL,
  `Lang` INT(11) NULL DEFAULT NULL,
  PRIMARY KEY (`Id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`notes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`notes` ;

CREATE TABLE IF NOT EXISTS `mydb`.`notes` (
  `idnotes` INT NOT NULL,
  `Contents` VARCHAR(255) NULL,
  `tasks_Id` INT(11) NOT NULL,
  `users_Id` VARCHAR(45) NOT NULL,
  `Uploadedtime` VARCHAR(25) NULL,
  PRIMARY KEY (`idnotes`, `tasks_Id`, `users_Id`),
  INDEX `fk_notes_tasks_idx` (`tasks_Id` ASC),
  INDEX `fk_notes_users1_idx` (`users_Id` ASC),
  CONSTRAINT `fk_notes_tasks`
    FOREIGN KEY (`tasks_Id`)
    REFERENCES `project461`.`tasks` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_notes_users1`
    FOREIGN KEY (`users_Id`)
    REFERENCES `project461`.`users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `project461` ;

-- -----------------------------------------------------
-- Table `project461`.`manager_project`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`manager_project` ;

CREATE TABLE IF NOT EXISTS `project461`.`manager_project` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT,
  `ProjectId` INT(11) NOT NULL,
  `users_Id` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Id`, `users_Id`),
  INDEX `fk_manager_project_users_idx` (`users_Id` ASC),
  CONSTRAINT `fk_manager_project_users`
    FOREIGN KEY (`users_Id`)
    REFERENCES `project461`.`users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `project461`.`projects`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`projects` ;

CREATE TABLE IF NOT EXISTS `project461`.`projects` (
  `Name` VARCHAR(45) NOT NULL,
  `manager_project_Id` INT(11) NOT NULL,
  `manager_project_users_Id` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`manager_project_Id`, `manager_project_users_Id`),
  INDEX `fk_projects_manager_project1_idx` (`manager_project_Id` ASC, `manager_project_users_Id` ASC),
  CONSTRAINT `fk_projects_manager_project1`
    FOREIGN KEY (`manager_project_Id` , `manager_project_users_Id`)
    REFERENCES `project461`.`manager_project` (`Id` , `users_Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `project461`.`project_task`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`project_task` ;

CREATE TABLE IF NOT EXISTS `project461`.`project_task` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT,
  `projects_manager_project_Id` INT(11) NOT NULL,
  `projects_manager_project_users_Id` VARCHAR(45) NOT NULL,
  `tasks_Id` INT(11) NOT NULL,
  PRIMARY KEY (`Id`, `projects_manager_project_Id`, `projects_manager_project_users_Id`, `tasks_Id`),
  INDEX `fk_project_task_projects1_idx` (`projects_manager_project_Id` ASC, `projects_manager_project_users_Id` ASC),
  INDEX `fk_project_task_tasks1_idx` (`tasks_Id` ASC),
  CONSTRAINT `fk_project_task_projects1`
    FOREIGN KEY (`projects_manager_project_Id` , `projects_manager_project_users_Id`)
    REFERENCES `project461`.`projects` (`manager_project_Id` , `manager_project_users_Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_project_task_tasks1`
    FOREIGN KEY (`tasks_Id`)
    REFERENCES `project461`.`tasks` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `project461`.`task_programmer`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`task_programmer` ;

CREATE TABLE IF NOT EXISTS `project461`.`task_programmer` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT,
  `TaskId` INT(11) NOT NULL,
  `users_Id` VARCHAR(45) NOT NULL,
  `tasks_Id` INT(11) NOT NULL,
  PRIMARY KEY (`Id`, `users_Id`, `tasks_Id`),
  INDEX `fk_task_programmer_users1_idx` (`users_Id` ASC),
  INDEX `fk_task_programmer_tasks1_idx` (`tasks_Id` ASC),
  CONSTRAINT `fk_task_programmer_users1`
    FOREIGN KEY (`users_Id`)
    REFERENCES `project461`.`users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_task_programmer_tasks1`
    FOREIGN KEY (`tasks_Id`)
    REFERENCES `project461`.`tasks` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `project461`.`task_requested_by_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `project461`.`task_requested_by_user` ;

CREATE TABLE IF NOT EXISTS `project461`.`task_requested_by_user` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT,
  `users_Id` VARCHAR(45) NOT NULL,
  `tasks_Id` INT(11) NOT NULL,
  PRIMARY KEY (`Id`, `users_Id`, `tasks_Id`),
  INDEX `fk_task_requested_by_user_users1_idx` (`users_Id` ASC),
  INDEX `fk_task_requested_by_user_tasks1_idx` (`tasks_Id` ASC),
  CONSTRAINT `fk_task_requested_by_user_users1`
    FOREIGN KEY (`users_Id`)
    REFERENCES `project461`.`users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_task_requested_by_user_tasks1`
    FOREIGN KEY (`tasks_Id`)
    REFERENCES `project461`.`tasks` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

INSERT INTO users (Id, Password, Type, Lang) values ('p', 'p', 'Programmer', 0);
INSERT INTO users (Id, Password, Type, Lang) values ('m', 'm', 'Manager', 0);
INSERT INTO users (Id, Password, Type, Lang) values ('mm', 'mm', 'Manager', 0);
