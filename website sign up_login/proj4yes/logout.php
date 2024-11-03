<?php
// Start the session to access session variables
session_start();

// Destroy all session data
session_destroy();

// Redirect the user to the login page
header("Location: login.php");
?>
// this logout.php file logs the user out by destroying the session data and then redirects them to the login page, ensuring they are redirected to the login page after logging out.