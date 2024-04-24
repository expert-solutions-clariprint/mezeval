<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<HTML>
		<HEAD>
		  <TITLE>Programme</TITLE>		
		</HEAD>
		<BODY BGCOLOR="#FFFFFF">
		<xsl:apply-templates select="/programme" />
		</BODY>
		</HTML>
	</xsl:template >
	<xsl:template match="programme" >
		<H1><xsl:value-of select="@name"/> (<xsl:value-of select="@annee"/>)</H1>
		<ol>
			<xsl:apply-templates select="section" />
		</ol>
	</xsl:template >
	<xsl:template match="section" >
		<li><xsl:value-of select="@name"/>
			<ol>
				<xsl:apply-templates select="section" />
			</ol>
		</li>
	</xsl:template >
</xsl:stylesheet>

