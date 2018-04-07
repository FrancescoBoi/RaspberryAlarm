<?php
$stored_password ='$2y$10$hqPSANTDdeC357ajPSIE3.B6GxXv7JNjdAUlqzayoCfpnw2wbz4zu';
$stored_uname =   '$2y$10$14U6jkdnqy/1mnS/8FFyCOP72GzjNl8PEGIvxJkW8uaG40IJ/Fvoi';
if( password_verify($_POST["user"],$stored_uname) && password_verify($_POST["pass"], $stored_password) )
    header("location:casa.html");
else
    header("location:login2.html");
?>
