using System;
using Azure.Identity;
using Azure.Storage.Blobs;
using Azure.Storage.Sas;

namespace LeanCode.StorageToken
{
    class Program
    {
        static void Main()
        {
            var credentials = new ClientSecretCredential(
                GetConfig("ARM_TENANT_ID"),
                GetConfig("ARM_CLIENT_ID"),
                GetConfig("ARM_CLIENT_SECRET"));
            var endpoint = $"https://{GetConfig("ARM_STORAGE_ACCOUNT_NAME")}.blob.core.windows.net";
            var client = new BlobServiceClient(new Uri(endpoint), credentials);

            var start = DateTimeOffset.UtcNow;
            var expires = start.AddHours(1);
            var key = client.GetUserDelegationKey(start, expires).Value;

            var sasBuilder = new BlobSasBuilder()
            {
                BlobContainerName = GetConfig("ARM_CONTAINER_NAME"),
                Resource = "b",
                ExpiresOn = expires,
                Protocol = SasProtocol.Https,
            };
            sasBuilder.SetPermissions(BlobSasPermissions.Read | BlobSasPermissions.Write);
            var sasToken = sasBuilder.ToSasQueryParameters(key, client.AccountName);
            System.Console.WriteLine(sasToken);
        }

        private static string GetConfig(string name)
        {
            var env = Environment.GetEnvironmentVariable(name);
            if (string.IsNullOrEmpty(env))
            {
                throw new ArgumentException($"The environment variable {name} is not specified.");
            }
            else
            {
                return env;
            }
        }
    }
}
