<?php
session_start();
if (!isset($_SESSION["user"])) {     //only authenticated users can access the content of this page 
   header("Location: login.php");   //the script redirects the user to the login page is the user session variable is not set :wrong user name in the login
}
?>
<!DOCTYPE html> 
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" integrity="sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi" crossorigin="anonymous">
    <link rel="stylesheet" href="style.css">
    <title>User Dashboard</title>
</head>
<body>
    <div class="container">
        <h1>Welcome to Dashboard</h1>
        <a href="logout.php" class="btn btn-warning">Logout</a>    //when you press logout the target is logout.php file
    </div>                                                         //class attribute assign 2 css bootstrap classes used to style the link as a button with a warning color
</body>
</html>