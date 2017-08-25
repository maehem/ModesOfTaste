<?php
    if ( $_POST['key'] === "h39sGb-jdow8bb%dd" ) {
        if( $_POST['ip'] ) {
            if (preg_match("/^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/")) {
                die ("invalid address");
            }
            echo "logged IP:". $_POST['ip'];
            echo "  mode:". $_POST['mode']. "<br />";
            $line = $_POST['mode'].': '.$_POST['ip'];
            file_put_contents('list.txt', $line, FILE_APPEND | LOCK_EX);

            exit();
        }
    }
    //else {
    //    die ("bad key");
    //}

    ?>
<html>
<body>

<form action = "<?php $_PHP_SELF ?>" method = "POST">
Key: <input type = "text" name = "key" />
IP: <input type = "text" name = "ip" />
Mode: <input type = "text" name = "mode" />
<input type = "submit" />
</form>

</body>
</html>
