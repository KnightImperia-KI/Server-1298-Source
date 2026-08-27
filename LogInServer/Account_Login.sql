USE [kn_online]
GO

/****** Object:  StoredProcedure [dbo].[ACCOUNT_LOGIN]    Script Date: 23.05.2026 20:03:01 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE OR ALTER PROCEDURE [dbo].[ACCOUNT_LOGIN]
@AccountID	varchar(50),
@Password	varchar(50),
@nRet		smallint OUTPUT
AS
/*
Author: Nero
*/

DECLARE @AccountID2 varchar(50), @Password2 varchar(50), @Authority smallint, @Char1 smallint, @Char2 smallint, @Char3 smallint
SELECT @Char1 = Authority FROM USERDATA WHERE strUserID = (SELECT strCharID1 FROM ACCOUNT_CHAR WHERE strAccountID = @AccountID and strCharID1 IS NOT NULL)
SELECT @Char2 = Authority FROM USERDATA WHERE strUserID = (SELECT strCharID2 FROM ACCOUNT_CHAR WHERE strAccountID = @AccountID and strCharID2 IS NOT NULL)
SELECT @Char3 = Authority FROM USERDATA WHERE strUserID = (SELECT strCharID3 FROM ACCOUNT_CHAR WHERE strAccountID = @AccountID and strCharID3 IS NOT NULL)
IF @Char1 = 255 or @Char2 = 255 or @Char3 = 255
BEGIN
	UPDATE TB_USER SET strAuthority = 255 where strAccountID = @AccountID
END
ELSE
BEGIN
	UPDATE TB_USER SET strAuthority = NULL where strAccountID = @AccountID
END

SELECT @AccountID2 = COUNT(strAccountID) FROM TB_USER WHERE strAccountID = @AccountID and iDays=6
IF @AccountID2 = 0
BEGIN
	SET @nRet = 2 -- Account doesnt exist.
	RETURN
END

SELECT @Password2 = strPasswd, @Authority = strAuthority FROM TB_USER WHERE strAccountID = @AccountID and iDays=6
IF @Password2 IS null or @Password2 != @Password
BEGIN
	SET @nRet = 3 -- Invalid password.
	RETURN
END
ELSE
IF @Authority = 255
BEGIN
	SET @nRet = 4 -- Account is banned.
	RETURN
END
ELSE
BEGIN
	DELETE CURRENTUSER WHERE strAccountID = @AccountID
	SET @nRet = 1
	RETURN
END
GO

