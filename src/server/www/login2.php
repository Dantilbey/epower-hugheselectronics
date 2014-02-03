<?php
   session_start();

if (isset($_POST['userid']) && isset($_POST['password'])
{
  // if the user has just tried to log in
  $userid = $_POST['userid'];
  $password = $_POST['password'];

  $db_conn = pg_connect('dbname=jelly_db user=jelly_user password=jelly');
  $query = "SELECT * FROM authorized_users WHERE name='$userid' AND password='$password' ";

  if (pg_num_rows($result) > 0)
  {
    // Records returned
  }

  if ($result->num_rows >0)  /*This is the area of difficulty.  I have tried different compares, but nothing is working.  Any ideas?  
It doesn't work with the current compare.  It will log everyone in if it is changed, say, to ($result >0) or something.  */
  {
    // if they are in the database register the user id
    $_SESSION['valid_user'] = $userid;
  }

  pg_close($db_conn);
 }

  if (isset($_SESSION['valid_user']))
  {
     main page....cool functionality
  }
  else
    {
       if (isset($userid))
        {
          echo 'Could not log you in.<br />';
        }
    else
       {
         echo 'You are not logged in.<br />';
        }

    Log in form goes here......
    }
 >?