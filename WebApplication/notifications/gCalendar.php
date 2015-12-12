<?php
require_once('config.inc.php');


$txt = "";
$txt .= "<h1>Headers</h1><table>";

foreach (getallheaders() as $name => $value) {
    $txt .= "<tr><td>$name</td><td> $value</td></tr>";
}

$txt .= "</table>";

$txt .= "<h1>POST</h1><table>";
foreach ($_POST as $name => $value) {
    $txt .= "<tr><td>$name</td><td> $value</td></tr>";
}
$txt .= "</table>";
echo($txt);

$mail = getPHPMailInstance();

die("Hello, world!");

$mail->From = 'webhook@timearc.nl';
$mail->FromName = 'Timearc google webhook';

$mail->addAddress('prof.pel.45@gmail.com');     // Add a recipient

$mail->addReplyTo('armin@timearc.nl', 'Timearc google webhook');
$mail->isHTML(true);  

$mail->Subject = 'Timearc webhook ' . date("F j, Y");
$mail->Body    = $txt;
$mail->AltBody = strip_tags($txt);

if($mail->send())
{
	echo("<p>Email send</p>");
}
else
{
	echo("<h1>Sending email failed!</h1><p>" . $mail->ErrorInfo) . "</p>";
}
	
