<?php
require __DIR__ . '\databaseconnect.php';

/**
 *
 * @package iamchidi webdev
 * @author Chidi Nkwocha <profchydon@gmail.com>
 * @copyright Copyright (c) 2013, iamchidi webdev
 */

class Database implements DatabaseConnectionInterface {
    public function createConnection($host,$database_name,$username,$password) {
        $dsn = 'mysql:host='.$host.';dbname='.$database_name;
        $pdo = new PDO($dsn, $username, $password);
        if ($pdo) {
          return true;
        }else {
          return false;
        }
    }
}

// $database = new Database();
// $host = "localhost";
// $database_name = "smashjob";
// $username = "root";
// $password = "";
// $database->createConnection($host,$database_name,$username,$password);
?>
