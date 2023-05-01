#!/usr/bin/php
<?php

$options = getopt("h", array("help"));
if (array_key_exists("h", $options) || array_key_exists("help", $options)) {  // help option handling
    printf("usage: php8.1 <path/parse.php> [--help]\n");
    printf("\tScript takes ippcode21 code from stdin and returns XML on stdout\n");
    printf("\t--help is optional argument, which displays this help\n");
    printf("contact: xfrlic00@stud.fit.vutbr.cz\n");
    exit(0);
}

$myFile = STDIN;

//initializing global variables
$order = 1;

// xml writer init
$xw = xmlwriter_open_memory();
xmlwriter_set_indent($xw, 4);
xmlwriter_set_indent_string($xw, "\t");
xmlwriter_start_document($xw, '1.0', 'UTF-8');
xmlwriter_start_element($xw, 'program'); //beggining of program element
addXMLAttr($xw, "language", "IPPcode22");

// file reading
$line = fgets($myFile);

// read until first line of text (comments excluded)
while (!feof($myFile) && preg_match("/^\s*$|^\s*#.*$/", $line)) {
    $line = fgets($myFile);
}


// Comments removal
$line = preg_replace("/#.*$/", "", $line);

// header check
if (!preg_match("/^\s*\.ippcode22\s*$/i", $line))
    exit(21);

// main loop
while (!feof($myFile)) {
    $line = fgets($myFile);
    if (preg_match("/(^#)|(^\s*$)/", $line))
        continue;
    parseLine($xw, $line);
}

// end and writeout
xmlwriter_end_element($xw); // end of program element
xmlwriter_end_document($xw); // printing XML
echo xmlwriter_output_memory($xw);
exit(0);


function addXMLAttr($xw, $name, $value)
{
    xmlwriter_start_attribute($xw, $name);
    xmlwriter_text($xw, $value);
    xmlwriter_end_attribute($xw);
}

// encapsulation for argument element in xml
function addArgToInstruction($xw, $i, $type, $value)
{
    xmlwriter_start_element($xw, "arg{$i}");
    addXMLAttr($xw, "type", $type);
    xmlwriter_text($xw, $value);
    xmlwriter_end_element($xw);
}


// lexical check for variable name
function parseVar($xw, $text, $i)
{
    if (!preg_match("/^[LTG]F@[a-zA-Z_\-\$&%\*!\?][a-zA-Z\d_\-\$&%\*!\?]*$/", $text))
        exit(23);
    addArgToInstruction($xw, $i, "var", $text);
    return 0;
}

// lexical check for symbol
function parseSymb($xw, $text, $i)
{
    if (preg_match("/^int|^string|^bool|^nil/", $text)) {
        parseLiteral($xw, $text, $i);
        return 0;
    }
    if (preg_match("/^LF|^GF|^TF/", $text)) {
        parseVar($xw, $text, $i);
        return 0;
    }
    exit(23);
}

// lexical checks for all literals (int, bool, string, nil)
function parseLiteral($xw, $text, $i)
{
    if (preg_match("/^int@[+\-]?(\d+|0x[\da-fA-F]+|0[0-7]+|0o[0-7]+)$/", $text)) {
        $str = preg_replace("/^int@/", "", $text);
        addArgToInstruction($xw, $i, "int", $str);
        return 0;
    }
    if (preg_match("/^string@/", $text)) {
        $str = preg_replace("/^string@/", "", $text);
        if (preg_match_all("/\\\\[0-9]{3}/", $text) != preg_match_all("/\\\\/", $text))
            exit(23);
        addArgToInstruction($xw, $i, "string", $str);
        return 0;
    }
    if (preg_match("/^bool@(true|false)$/", $text)) {
        $str = preg_replace("/^bool@/", "", $text);
        addArgToInstruction($xw, $i, "bool", $str);
        return 0;
    }
    if ($text == "nil@nil") {
        addArgToInstruction($xw, $i, "nil", "nil");
        return 0;
    }
    exit(23);
}

// lexical check for label
function parseLabel($xw, $text, $i)
{
    if (!preg_match("/^[a-z_\-\$&%\*!\?][a-z0-9_\-\$&%\*!\?]*$/i", $text))
        exit(23);
    addArgToInstruction($xw, $i, "label", $text);
}

//lexical check for type
function parseType($xw, $text, $i)
{
    if (!preg_match("/^int$|^bool$|^string$|^nil$/", $text))
        exit(23);
    addArgToInstruction($xw, $i, "type", $text);
}


// function does all checks for line
// syntax check for instruction name, and lexical checks for all attributes
function parseLine($xw, $line)
{
    $INSTRUCTIONS = array(
        "MOVE" => array("parseVar", "parseSymb"),
        "CREATEFRAME" => array(),
        "PUSHFRAME" => array(),
        "POPFRAME" => array(),
        "DEFVAR" => array("parseVar"),
        "CALL" => array("parseLabel"),
        "RETURN" => array(),
        "PUSHS"  => array("parseSymb"),
        "POPS"  => array("parseVar"),
        "ADD"  => array("parseVar", "parseSymb", "parseSymb"),
        "SUB"  => array("parseVar", "parseSymb", "parseSymb"),
        "MUL"  => array("parseVar", "parseSymb", "parseSymb"),
        "IDIV"  => array("parseVar", "parseSymb", "parseSymb"),
        "LT"  => array("parseVar", "parseSymb", "parseSymb"),
        "GT"  => array("parseVar", "parseSymb", "parseSymb"),
        "EQ"  => array("parseVar", "parseSymb", "parseSymb"),
        "AND"  => array("parseVar", "parseSymb", "parseSymb"),
        "OR"  => array("parseVar", "parseSymb", "parseSymb"),
        "NOT"  => array("parseVar", "parseSymb"),
        "INT2CHAR"  => array("parseVar", "parseSymb"),
        "STRI2INT"  => array("parseVar", "parseSymb", "parseSymb"),
        "READ"  => array("parseVar", "parseType"),
        "WRITE" => array("parseSymb"),
        "CONCAT"  => array("parseVar", "parseSymb", "parseSymb"),
        "STRLEN"  => array("parseVar", "parseSymb"),
        "GETCHAR"  => array("parseVar", "parseSymb", "parseSymb"),
        "SETCHAR"  => array("parseVar", "parseSymb", "parseSymb"),
        "TYPE"  => array("parseVar", "parseSymb"),
        "LABEL" => array("parseLabel"),
        "JUMP" => array("parseLabel"),
        "JUMPIFEQ" => array("parseLabel", "parseSymb", "parseSymb"),
        "JUMPIFNEQ" => array("parseLabel", "parseSymb", "parseSymb"),
        "EXIT" => array("parseSymb"),
        "DPRINT" => array("parseSymb"),
        "BREAK" => array(),
    );

    // comment and multiple whitespaces removal
    $patterns = array("/#.*$/", "/\s*$/", "/^\s*/", "/\s+/");
    $replace = array("", "", "", " ");
    $line = preg_replace($patterns, $replace, $line);
    
    // spliting line by whitespaces
    $words = explode(" ", $line);
    $instruction = strtoupper($words[0]);

    // finding instruction
    if (!array_key_exists($instruction, $INSTRUCTIONS))
        exit(22);

    xmlwriter_start_element($xw, 'instruction'); // begining of instruction element
    addXMLAttr($xw, "order", $GLOBALS["order"]++);
    addXMLAttr($xw, "opcode", $instruction);


    $attributeTypes = $INSTRUCTIONS[$instruction];
    $i = 1;

    // checking number of required arguments
    if (count($words) != 1 + count($attributeTypes)) exit(23);
    
    // calling lexical checks for each attribute
    foreach ($attributeTypes as $atr)
        $atr($xw, $words[$i], $i++);

    xmlwriter_end_element($xw); // end of instruction element

    return 0;
}

?>