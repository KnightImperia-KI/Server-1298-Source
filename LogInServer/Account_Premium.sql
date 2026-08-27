USE [kn_online]
GO

/****** Object:  StoredProcedure [dbo].[ACCOUNT_PREMIUM]    Script Date: 23.05.2026 20:03:23 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER OFF
GO


CREATE OR ALTER PROCEDURE [dbo].[ACCOUNT_PREMIUM] 
@AccountID	varchar(50),
@PremiumDays	smallint OUTPUT
AS
/*
Author: Nero
*/
DECLARE @PremiumDays2 smallint
SELECT @PremiumDays2 = Count(strAccountID) FROM PREMIUM_SERVICE Where strAccountID = @AccountID
IF @PremiumDays2 = 0
BEGIN
	INSERT INTO PREMIUM_SERVICE (strAccountID, strType, nDays) VALUES (@AccountID, 1, 3)
END

SELECT @PremiumDays = nDays FROM PREMIUM_SERVICE Where strAccountID = @AccountID
IF @PremiumDays < 1
	SET @PremiumDays = 0
BEGIN
	SET @PremiumDays = @PremiumDays
END
GO

