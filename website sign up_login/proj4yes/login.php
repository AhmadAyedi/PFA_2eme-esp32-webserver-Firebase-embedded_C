<?php
session_start();
if (isset($_SESSION["user"])) {
   header("Location: index.php");
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <!-- Set the character encoding to UTF-8 -->
    <meta charset="UTF-8">
    <!-- Ensure proper rendering and compatibility with IE -->
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <!-- Set the viewport width to the device width and initial scale to 1.0 -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Set the title of the HTML document -->
    <title>Login Form</title>
    <!-- Include Bootstrap CSS from CDN for styling -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" integrity="sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi" crossorigin="anonymous">
    <!-- Include custom CSS for additional styling -->
    <link rel="stylesheet" href="style.css">
</head>

<body>
    <!-- Container for the content -->
    <div class="container">
        <?php
        // Check if the login form is submitted
        if (isset($_POST["login"])) {
           // Retrieve email and password from the submitted form
           $email = $_POST["email"];
           $password = $_POST["password"];
           
           // Include the database connection file
            require_once "database.php";
            
            // Prepare SQL query to select user with provided email
            $sql = "SELECT * FROM users WHERE email = '$email'";
            $result = mysqli_query($conn, $sql);
            
            // Fetch user data
            $user = mysqli_fetch_array($result, MYSQLI_ASSOC);
            
            // Check if user exists
            if ($user) {
                // Verify the password using password_verify function
                if (password_verify($password, $user["password"])) {
                    // Start a new session
                    session_start();
                    
                    // Set a session variable indicating user is logged in
                    $_SESSION["user"] = "yes";
                    
                    // Redirect user to the dashboard page
                    header("Location: index.php");
                    // Stop further execution
                    die();
                } else {
                    // Display error message if password does not match
                    echo "<div class='alert alert-danger'>Password does not match</div>";
                }
            } else {
                // Display error message if email does not match
                echo "<div class='alert alert-danger'>Email does not match</div>";
            }
        }
        ?>
        <!-- Login form -->
        <form action="login.php" method="post">
            <div class="form-group">
                <input type="email" placeholder="Enter Email:" name="email" class="form-control">
            </div>
            <div class="form-group">
                <input type="password" placeholder="Enter Password:" name="password" class="form-control">
            </div>
            <div class="form-btn">
                <input type="submit" value="Login" name="login" class="btn btn-primary">
            </div>
        </form>
        <!-- Link to registration page -->
        <div><p>Not registered yet <a href="registration.php">Register Here</a></p></div>
    </div>
</body>

</html>