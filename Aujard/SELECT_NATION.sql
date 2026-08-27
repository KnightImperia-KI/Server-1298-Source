SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROCEDURE dbo.SELECT_NATION
@AccountID	varchar(50),
@Password	varchar(50),
@nRet		smallint OUTPUT
AS
/*
Author: Nero
*/
DECLARE @Row tinyint, @Nation smallint, @CharNum tinyint, @Password2 varchar(50)
SELECT @Password2 = strPasswd FROM TB_USER WHERE strAccountID = @AccountID and iDays=6
IF @Password2 IS null or @Password2 != @Password
/*
nRet = 20 -- Your current region is not able to login to this server. Plaese try other servers. Thank you.
*/
BEGIN
	SET @nRet = -1
	RETURN
END

SELECT @Row = COUNT(strAccountID)  FROM ACCOUNT_CHAR WHERE strAccountID =  @AccountID
SELECT @Nation = bNation, @CharNum = bCharNum FROM ACCOUNT_CHAR WHERE strAccountID = @AccountID
IF @Row = 0 or @CharNum = 0
BEGIN
	SET @nRet = 0
	RETURN
END
ELSE
BEGIN
	SET @nRet = @Nation
	RETURN
END
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO